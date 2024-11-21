#pragma once
#include <queue>
#include "includes/TcpCommand.hpp"
#include "includes/TcpParser.hpp"
#include "includes/TcpExecuter.hpp"
#include "includes/TcpCommandFactory.hpp"
#include "includes/ParsedTcpCommand.hpp"
#include <memory>
#include <iostream>
#include <string>

struct CommandManager
{
	CommandManager();
	std::queue<std::unique_ptr<TcpCommand>> _command_queue;
	void push(std::unique_ptr<TcpCommand> command);
	std::unique_ptr<TcpCommand> pop();
	std::string execute_command(std::string command);
private:
	TcpParser _parser;
	TcpCommandFactory _factory;
	TcpExecuter _executer;
};

