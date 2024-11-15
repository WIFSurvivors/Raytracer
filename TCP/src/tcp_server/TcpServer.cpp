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
        read_message();
    } else {
        std::cout << "Accept failed: " << error.message() << std::endl;
    }
    //if (!error && !_is_stopped) {
    //    std::cout << "-- connection accepted\n";
    //    char buffer[1024];
    //    boost::system::error_code read_error;
    //    size_t length = _socket.read_some(boost::asio::buffer(buffer), read_error);
    //    if (!read_error) {
    //        std::string msg = std::string(buffer, length);
    //        std::cout << "Data received: " << msg << std::endl;
    //        // TODO: Parse TCP Packet and get command
    //        TcpParser parser;
    //        std::unique_ptr<TcpCommand> command = parser.parse(msg);
    //        // TODO: Give command to the command Executer
    //        TcpExecuter executer;
    //        if (command) {
    //            // Give command to the command Executer
    //            TcpExecuter executer;
    //            int return_value = executer.execute(*command);
    //            boost::asio::write(_socket, boost::asio::buffer(std::to_string(return_value)));
    //        } else {
    //            std::cout << "Failed to parse command" << std::endl;
    //        }
    //    } else {
    //        std::cout << "Receive failed: " << read_error.message() << std::endl;
    //    }
    //    _socket.close();
    //    do_accept();
    //} else {
    //    std::cout << "Accept failed: " << error.message() << std::endl;
    //}
}

void TcpServer::read_message() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_buffer), [this, self](const boost::system::error_code& error, std::size_t length) {
        if (!error && !_is_stopped) {
            std::string msg(_buffer.data(), length);
            std::cout << "Data received: " << msg << std::endl;
            TcpParser parser;
            std::unique_ptr<TcpCommand> command = parser.parse(msg);
            if (command) {
                TcpExecuter executer;
                int return_value = executer.execute(*command);
                boost::asio::write(_socket, boost::asio::buffer(std::to_string(return_value)));
            } else {
                std::cout << "Failed to parse command" << std::endl;
            }
            read_message(); // Continue reading messages
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
    std::cout << "Server stopped" << std::endl;
}

int main() {
    try {
        boost::asio::io_context io_context;
        auto server = std::make_shared<TcpServer>(io_context, 51234);
        server->start();
        std::this_thread::sleep_for(std::chrono::seconds(10));
        server->stop();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}