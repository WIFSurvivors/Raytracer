#include "includes/TcpServer.hpp"


TcpServer::TcpServer(boost::asio::io_context& io_context, int port)
    : _io_context(io_context),
      _acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
      _socket(io_context),
      _is_stopped(false) {}

TcpServer::~TcpServer() {
    if (_server_thread.joinable()) {
        stop();
        _server_thread.join();
    }
}
void TcpServer::start() {
    std::cout << "-- waiting for incoming connections...\n";
    do_accept();
    _server_thread = std::thread([this]() { _io_context.run(); });
}

void TcpServer::do_accept() {
    _acceptor.async_accept(_socket, [this](const boost::system::error_code& error) {
        handle_accept(error);
    });
}

void TcpServer::handle_accept(const boost::system::error_code& error) {
    if (!error && !_is_stopped) {
        std::cout << "-- connection accepted\n";
        char buffer[1024];
        boost::system::error_code read_error;
        size_t length = _socket.read_some(boost::asio::buffer(buffer), read_error);
        if (!read_error) {
            std::cout << "Data received: " << std::string(buffer, length) << std::endl;
            boost::asio::write(_socket, boost::asio::buffer("Hello from server!"));
        } else {
            std::cout << "Receive failed: " << read_error.message() << std::endl;
        }
        _socket.close();
        do_accept();
    } else {
        std::cout << "Accept failed: " << error.message() << std::endl;
    }
}

void TcpServer::stop() {
    _is_stopped = true;
    _acceptor.close();
    _socket.close();
    std::cout << "Server stopped" << std::endl;
}

int main() {
    try {
        boost::asio::io_context io_context;
        TcpServer server(io_context, 51234);
        server.start();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}