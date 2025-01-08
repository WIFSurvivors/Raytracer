#include "includes/tcp_server/TcpServer.hpp"
#include "includes/tcp_server/CommandManager.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include <string>

using namespace RT;

TcpServer::TcpServer(int port, Engine *e)
    : _io_context(), _acceptor(_io_context, tcp::endpoint(tcp::v4(), port)),
      _socket(_io_context), _is_stopped(false), _engine(e),
      _command_manager(this, e) {}

TcpServer::~TcpServer() {
  if (_server_thread.joinable()) {
    stop();
    _server_thread.join();
  }
}
void TcpServer::start() {
  LOG_TCP("-- waiting for incoming connections...");
  do_accept();
  _server_thread = std::thread([this]() { _io_context.run(); });
}

void TcpServer::do_accept() {
  _acceptor.async_accept(_socket,
                         [this](const boost::system::error_code &error) {
                           LOG_TCP("Accepting connection");
                           handle_accept(error);
                         });
}

void TcpServer::execute_command() { _command_manager.execute_command(); }
void TcpServer::handle_accept(const boost::system::error_code &error) {
  if (!error && !_is_stopped) {
    LOG_TCP("-- connection accepted");
    read_message();
  } else {
    LOG_ERROR("Accept failed: " + error.message());
  }
}
void TcpServer::send_message(std::string message) {
  boost::asio::write(_socket, boost::asio::buffer(message));
}

void TcpServer::read_message() {
  auto self(shared_from_this());
  _socket.async_read_some(
      boost::asio::buffer(_buffer),
      [this, self](const boost::system::error_code &error, std::size_t length) {
        if (!error && !_is_stopped) {
          std::string msg(_buffer.data(), length);
          LOG_TCP("Data received: " + msg);
          auto command_decoded = _command_manager.decode_command(msg);
          auto command = _command_manager.create_command(*command_decoded);
          if (command) {
            _command_manager.push(std::move(command));
          } else {
            LOG_ERROR("Failed to parse command");
            send_message("Failed to parse command");
          }
          read_message();
        } else {
          LOG_ERROR("Receive failed: " + error.message());
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
  LOG_TCP("Server stopped");
}
