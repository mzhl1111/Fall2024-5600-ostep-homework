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
#include <future>

void handleConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    try {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nHello";
        boost::asio::write(*socket, boost::asio::buffer(response));
    } catch (std::exception &e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
}

int main() {
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

    while (true) {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
        acceptor.accept(*socket);

        // Use std::async to handle the connection asynchronously
        std::async(std::launch::async, [socket]() mutable {
            handleConnection(socket);
        });
    }

    return 0;
}
