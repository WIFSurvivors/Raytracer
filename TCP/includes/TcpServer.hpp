#pragma once
#include <iostream>
#include <boost/asio.hpp>

#include <cstring>

struct TcpServer {
  TcpServer();
  void start(int port);
  void stop();
private:
};
