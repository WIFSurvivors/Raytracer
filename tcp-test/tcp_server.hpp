#pragma once
#include <boost/asio.hpp>


struct tcp_server {
    
    typedef boost::asio::ip::tcp b_tcp;

    tcp_server(unsigned short port);

    void run() throw b_tcp;         

private:
    unsigned short _port;
    b_tcp::socket _socket;

    
};
