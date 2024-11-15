#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <cstring>
#include <thread>
#include <array>
#include <chrono>
#include "includes/TcpParser.hpp"
#include "includes/TcpExecuter.hpp"

using boost::asio::ip::tcp;

struct TcpServer : public std::enable_shared_from_this<TcpServer> {
  TcpServer(boost::asio::io_context &io_context, int port);
  ~TcpServer();
  void start();
  void stop();

private:
  void do_accept();
  void handle_accept(const boost::system::error_code &error);
  void read_message();
  boost::asio::io_context &_io_context;
  boost::asio::ip::tcp::acceptor _acceptor;
  boost::asio::ip::tcp::socket _socket;
  std::array<char, 1024> _buffer;
  std::thread _server_thread;
  bool _is_stopped;
};
