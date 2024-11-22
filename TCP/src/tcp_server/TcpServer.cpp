
#include "includes/TcpServer.hpp"

TcpServer::TcpServer(int port)
    : _io_context(),
      _acceptor(_io_context, tcp::endpoint(tcp::v4(), port)),
      _socket(_io_context),
      _is_stopped(false),
      _command_manager()
      {}

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
        read_message();
    } else {
        std::cout << "Accept failed: " << error.message() << std::endl;
    }
}

void TcpServer::read_message() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_buffer), [this, self](const boost::system::error_code& error, std::size_t length) {
        if (!error && !_is_stopped) {
            std::string msg(_buffer.data(), length);
            std::cout << "Data received: " << msg << std::endl;
            std::string return_value = _command_manager.execute_command(msg);
            boost::asio::write(_socket, boost::asio::buffer(return_value));
            read_message(); 
        } else {
            std::cout << "Receive failed: " << error.message() << std::endl;
            _socket.close();
            do_accept();
        }
    });
}

void TcpServer::stop() {
    _is_stopped = true;
    _acceptor.close();
    _socket.close();
    _io_context.stop();
    std::cout << "Server stopped" << std::endl;
}

//not really working as expected 
//#ifndef TOPLEVEL
//int main() {
//    try {
//        auto server = std::make_shared<TcpServer>(51234);
//        server->start();
//        std::this_thread::sleep_for(std::chrono::seconds(1000));
//      server->stop();
//   } catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//    return 0;
//}
//
//#endif