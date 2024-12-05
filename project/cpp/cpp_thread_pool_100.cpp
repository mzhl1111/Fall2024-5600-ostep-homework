#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <boost/asio.hpp>
#include <memory>

class ThreadPool {
public:
    ThreadPool(size_t maxThreads) : stop(false) {
        for (size_t i = 0; i < maxThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

void handleConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    try {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nHello";
        boost::asio::write(*socket, boost::asio::buffer(response));
    } catch (std::exception &e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
}

int main() {
    const size_t maxThreads = 100; // Set the maximum number of threads
    ThreadPool threadPool(maxThreads);

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

    while (true) {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
        acceptor.accept(*socket);

        threadPool.enqueue([socket]() mutable {
            handleConnection(socket);
        });
    }

    return 0;
}
