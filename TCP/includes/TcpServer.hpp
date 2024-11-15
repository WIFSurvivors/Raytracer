#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <cstring>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

struct TcpServer {
  TcpServer(boost::asio::io_context &io_context, int port);
  ~TcpServer();
  void start();
  void stop();

private:
  void do_accept();
  void handle_accept(const boost::system::error_code &error);
  boost::asio::io_context &_io_context;
  boost::asio::ip::tcp::acceptor _acceptor;
  boost::asio::ip::tcp::socket _socket;
  std::thread _server_thread;
  bool _is_stopped;
};
