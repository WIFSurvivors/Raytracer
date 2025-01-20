#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <array>
#include <memory>
#include <string>
#include "includes/tcp_server/CommandManager.hpp"

using boost::asio::ip::tcp;

using RT::Engine;
/// @brief TcpServer that listens for incoming tcp connections.
/// The TcpServer listens for incoming tcp connections and reads messages from the client.
/// The messages are decoded and executed as commands.
struct TcpServer : public std::enable_shared_from_this<TcpServer> {
  /// @brief Constructor for the TcpServer.
  /// @param port The port that the server listens on.
  /// @param e The engine that the commands are executed on.
  explicit TcpServer(int port, RT::Engine *e);
  /// @brief Destructor for the TcpServer.
  ~TcpServer();
  /// @brief Start the server.
  /// The server starts listening for incoming connections.
  void start();
  /// @brief Stop the server.
  /// The server stops listening for incoming connections.
  void stop();
  /// @brief Execute commands.
  /// The commands are executed by calling the execute_commands function of the command manager.
  void execute_commands();
  /// @brief Execute a command.
  /// The command is executed by calling the execute_command function of the command manager.
  void execute_command();
  /// @brief Send a message.
  /// The message is sent to the client.
  /// @param message The message to send.
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
