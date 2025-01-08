#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define EXPORT_JSON_COMMAND "ExportJson"
class ExportJsonCommand : public TcpCommand {
public:
  explicit ExportJsonCommand(std::string json_path) : _json_path(json_path) {}
  std::string execute(RT::Engine *engine) override;
  std::string undo() override;
  inline std::string get_json_path() const { return _json_path; }
  inline void set_json_path(std::string json_path) { _json_path = json_path; }

private:
  std::string _json_path;
};
