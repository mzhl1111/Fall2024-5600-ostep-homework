#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <memory>

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
    boost::asio::thread_pool threadPool(maxThreads);

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

    while (true) {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
        acceptor.accept(*socket);

        boost::asio::post(threadPool, [socket]() mutable {
            handleConnection(socket);
        });
    }

    threadPool.join();
    return 0;
}
