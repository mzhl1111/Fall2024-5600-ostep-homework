#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t poolSize)
        : stop(false) {
        for (size_t i = 0; i < poolSize; ++i) {
            workers.emplace_back([this] {
                for (;;) {
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

    void enqueueTask(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::move(task));
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

std::atomic<int> responseCounter(0);

void handleConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    try {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
        boost::asio::write(*socket, boost::asio::buffer(response));
        responseCounter.fetch_add(1, std::memory_order_relaxed);
    } catch (std::exception &e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
}

int main() {
    const size_t poolSize = 4;  
    ThreadPool threadPool(poolSize);

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

    for (;;) {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
        acceptor.accept(*socket);
        threadPool.enqueueTask([socket]() mutable {
            handleConnection(socket);
        });
    }

    return 0;
}
