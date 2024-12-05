#include <boost/asio.hpp>
#include <iostream>
#include <coroutine>
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>

using boost::asio::ip::tcp;

std::atomic<int> responseCounter(0);
std::atomic<int> activeCoroutines(0);
int maxCoroutines = 10;
std::queue<std::shared_ptr<tcp::socket>> taskQueue;
std::mutex queueMutex;

boost::asio::awaitable<void> handleConnection(std::shared_ptr<tcp::socket> socket) {
    try {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
        co_await boost::asio::async_write(*socket, boost::asio::buffer(response), boost::asio::use_awaitable);
        responseCounter.fetch_add(1, std::memory_order_relaxed);
    } catch (std::exception &e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
    activeCoroutines.fetch_sub(1, std::memory_order_relaxed);

    // Check if there are tasks in the queue
    std::shared_ptr<tcp::socket> nextSocket;
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!taskQueue.empty()) {
            nextSocket = taskQueue.front();
            taskQueue.pop();
        }
    }
    if (nextSocket) {
        boost::asio::co_spawn(co_await boost::asio::this_coro::executor, handleConnection(nextSocket), boost::asio::detached);
        activeCoroutines.fetch_add(1, std::memory_order_relaxed);
    }
}

boost::asio::awaitable<void> acceptConnections(tcp::acceptor& acceptor) {
    for (;;) {
        auto socket = std::make_shared<tcp::socket>(co_await acceptor.async_accept(boost::asio::use_awaitable));
        bool spawnNewCoroutine = false;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (activeCoroutines.load(std::memory_order_relaxed) < maxCoroutines) {
                spawnNewCoroutine = true;
                activeCoroutines.fetch_add(1, std::memory_order_relaxed);
            } else {
                taskQueue.push(socket);
            }
        }
        if (spawnNewCoroutine) {
            boost::asio::co_spawn(acceptor.get_executor(), handleConnection(socket), boost::asio::detached);
        }
    }
    co_return;
}

int main(int argc, char* argv[]) {
    try {
        if (argc > 1) {
            maxCoroutines = std::stoi(argv[1]);
        }

        boost::asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));

        boost::asio::co_spawn(ioContext, acceptConnections(acceptor), boost::asio::detached);

        ioContext.run();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
