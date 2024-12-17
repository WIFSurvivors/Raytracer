#include "includes/tcp_server/TcpServer.hpp"
#include "includes/tcp_server/CommandManager.hpp"
#include "includes/Engine.hpp"
TcpServer::TcpServer(int port, Engine *e)
    : _io_context(),
      _acceptor(_io_context, tcp::endpoint(tcp::v4(), port)),
      _socket(_io_context),
      _is_stopped(false),
      _engine(e),
    _command_manager(this,e)
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
    std::cout << "Server started" << std::endl;
    _server_thread = std::thread([this]() { _io_context.run(); });
}

void TcpServer::do_accept() {
    std::cout << "Starting async connection" << std::endl;
    _acceptor.async_accept(_socket, [this](const boost::system::error_code& error) {
        std::cout << "Accepting connection" << std::endl;
        handle_accept(error);
    });
    std::cout << "Connection accepted" << std::endl;
}

void TcpServer::execute_command() {
    _command_manager.execute_command();
}
void TcpServer::handle_accept(const boost::system::error_code& error) {
    if (!error && !_is_stopped) {
        std::cout << "-- connection accepted\n";
        read_message();
    } else {
        std::cout << "Accept failed: " << error.message() << std::endl;
    }
}
void TcpServer::send_message(std::string message) {
    boost::asio::write(_socket, boost::asio::buffer(message));
}

void TcpServer::read_message() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_buffer), [this, self](const boost::system::error_code& error, std::size_t length) {
        if (!error && !_is_stopped) {
            std::string msg(_buffer.data(), length);
            std::cout << "Data received: " << msg << std::endl;
            auto command_decoded = _command_manager.decode_command(msg);
            auto command = _command_manager.create_command(*command_decoded);
            if(command){
                _command_manager.push(std::move(command));
            }
            else{
                std::cout << "Failed to parse command" << std::endl;
                send_message("Failed to parse command");
            }
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
