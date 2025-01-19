#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/tcp_server/TcpParser.hpp"
#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/tcp_server/TcpCommandFactory.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
// #include "includes/Engine.hpp"
#include <queue>
#include <memory>
#include <iostream>
#include <string>
#include <mutex>
#include <stack>

class TcpServer;

using RT::Engine;

struct CommandManager {
  CommandManager(TcpServer *tcp_server, RT::Engine *engine)
      : _tcp_server(tcp_server), _engine(engine),
        _command_queue(std::queue<std::unique_ptr<TcpCommand>>()) {}
  void push(std::unique_ptr<TcpCommand> command);
  std::unique_ptr<TcpCommand> pop();
  std::string execute_command(std::string command);
  void execute_commands();
  inline auto decode_command(std::string command) {
    return _parser.parse(command);
  }
  auto inline create_command(ParsedTcpCommand parsed_command) {
    return _factory.create_command(parsed_command);
  }
  void undo_command(int number);
  void execute_command();
  ~CommandManager() = default;

private:
  std::queue<std::unique_ptr<TcpCommand>> _command_queue{};
  std::stack<std::unique_ptr<UndoableCommand>> _undo_stack{};
  TcpParser _parser{};
  TcpCommandFactory _factory{};
  TcpExecuter _executer{};
  RT::Engine *_engine;
  TcpServer *_tcp_server;
};
