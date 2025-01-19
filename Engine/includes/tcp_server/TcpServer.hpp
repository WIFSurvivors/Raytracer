#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <array>
#include <memory>
#include <string>
#include "includes/tcp_server/CommandManager.hpp"

using boost::asio::ip::tcp;

using RT::Engine;

struct TcpServer : public std::enable_shared_from_this<TcpServer> {
  explicit TcpServer(int port, RT::Engine *e);
  ~TcpServer();
  void start();
  void stop();
  void execute_commands();
  void execute_command();
  void send_message(std::string message);

private:
  void do_accept();
  void handle_accept(const boost::system::error_code &error);
  void read_message();
  boost::asio::io_context _io_context{};
  boost::asio::ip::tcp::acceptor _acceptor;
  boost::asio::ip::tcp::socket _socket;
  std::array<char, 4096> _read_buffer;
  std::thread _server_thread;
  bool _is_stopped;
  RT::Engine *_engine;
  CommandManager _command_manager;
};
