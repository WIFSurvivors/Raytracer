#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <iostream>
#include <string>

#define IMPORT_JSON_COMMAND "ImportJson"
class importJsonCommand : public TcpCommand
{
public:
    explicit importJsonCommand(std::string json_path) : _json_path(json_path) {}
    std::string execute(Engine * engine) override;
    int undo() override;

private:
    std::string _json_path;

};
