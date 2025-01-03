#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define IMPORT_JSON_COMMAND "ImportJson"
class importJsonCommand : public TcpCommand
{
public:
    explicit importJsonCommand(std::string json_path) : _json_path(json_path) {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
    inline std::string get_json_path() const { return _json_path; }
    inline void set_json_path(std::string json_path) { _json_path = json_path; }
private:
    std::string _json_path;

};
