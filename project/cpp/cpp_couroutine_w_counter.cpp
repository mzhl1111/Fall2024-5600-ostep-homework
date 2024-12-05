#include <boost/asio.hpp>
#include <iostream>
#include <coroutine>
#include <memory>
#include <atomic>

using boost::asio::ip::tcp;

std::atomic<int> responseCounter(0);

boost::asio::awaitable<void> handleConnection(std::shared_ptr<tcp::socket> socket) {
    try {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
        co_await boost::asio::async_write(*socket, boost::asio::buffer(response), boost::asio::use_awaitable);
        responseCounter.fetch_add(1, std::memory_order_relaxed);
    } catch (std::exception &e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
}

boost::asio::awaitable<void> acceptConnections(tcp::acceptor& acceptor, int coroutineCount) {
    for (int i = 0; i < coroutineCount; ++i) {
        auto socket = std::make_shared<tcp::socket>(acceptor.get_executor().context());
        co_await acceptor.async_accept(*socket, boost::asio::use_awaitable);
        boost::asio::co_spawn(acceptor.get_executor(), handleConnection(socket), boost::asio::detached);
    }
    co_return;
}

int main(int argc, char* argv[]) {
    try {
        int coroutineCount = 1;
        if (argc > 1) {
            coroutineCount = std::stoi(argv[1]);
        }

        boost::asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));

        boost::asio::co_spawn(ioContext, acceptConnections(acceptor, coroutineCount), boost::asio::detached);

        ioContext.run();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
