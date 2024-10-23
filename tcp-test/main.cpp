#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// -- install netcat (ubuntu):
// sudo apt update
// sudo apt install netcat
// -- send tcp messages on port 51234 like this:
// echo "Hewwo :3" | nc 127.0.0.1 51234

int main() {
  try {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 51234));
    std::cout << "-- waiting for incoming connections...\n";

    tcp::socket socket(io_context);
    acceptor.accept(socket);
    std::cout << "-- connection accepted\n";

    // im an literally insane rn

    char buffer[1024];
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(buffer), error);
    if (!error) {
      std::cout << "Data received: " << std::string(buffer, length)
                << std::endl;
    } else {
      std::cout << "Receive failed: " << error.message() << std::endl;
    }

    socket.close();
  } catch (std::exception &e) {
    std::cerr << "-- exception: " << e.what() << std::endl;
  }

  return 0;
}