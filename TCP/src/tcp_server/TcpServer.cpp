#include "includes/TcpServer.hpp"

using boost::asio::ip::tcp;

void TcpServer::start(int port) {
    // int server_fd, new_socket;
    // struct sockaddr_in address;
    // int opt = 1;
    // int addrlen = sizeof(address);
// 
    // // Creating socket file descriptor
    // if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    //     perror("socket failed");
    //     exit(EXIT_FAILURE);
    // }
// 
    // // Forcefully attaching socket to the port
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    // address.sin_family = AF_INET;
    // address.sin_addr.s_addr = INADDR_ANY;
    // address.sin_port = htons(port);
// 
    // // Forcefully attaching socket to the port
    // if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    //     perror("bind failed");
    //     exit(EXIT_FAILURE);
    // }
    // if (listen(server_fd, 3) < 0) {
    //     perror("listen");
    //     exit(EXIT_FAILURE);
    // }
    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
    //     perror("accept");
    //     exit(EXIT_FAILURE);
    // }
// 
    // std::cout << "Server started on port " << port << std::endl;
// 
    // // Close the socket
    // close(new_socket);
    // close(server_fd);#
    try{
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 51234));
        std::cout << "-- waiting for incoming connections...\n";

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "-- connection accepted\n";

        char buffer[1024];
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(buffer), error);
        if(!error){
            std::cout << "Data received: " << std::string(buffer, length) << std::endl;
        }
        else{
            std::cout << "Receive failed: " << error.message() << std::endl;
        }

        socket.close();
    }
    catch(std::exception& e){
        std::cerr << "-- exception: " << e.what() << std::endl;
    }

}

TcpServer::TcpServer()
{

}

void TcpServer::stop() {
    std::cout << "Server stopped" << std::endl;
}

int main() {
    TcpServer server;
    server.start(8080);
    server.stop();
    return 0;
}