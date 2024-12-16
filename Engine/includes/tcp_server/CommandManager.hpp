#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/tcp_server/TcpParser.hpp"
#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/tcp_server/TcpCommandFactory.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
#include <queue>
#include <memory>
#include <iostream>
#include <string>


struct CommandManager
{
	explicit CommandManager(Engine * engine) : _engine(engine) {}
	std::queue<std::unique_ptr<TcpCommand>> _command_queue;
	void push(std::unique_ptr<TcpCommand> command);
	std::unique_ptr<TcpCommand> pop();
	std::string execute_command(std::string command);
private:
	TcpParser _parser{};
	TcpCommandFactory _factory{};
	TcpExecuter _executer{};
	Engine * _engine;
};
