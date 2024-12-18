#pragma once
#include "includes/component/Component.hpp"
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/json.hpp>

#define GET_COMPONENTS_COMMAND "GetComponents"

struct IComponent;

struct GetComponentsCommand : public TcpCommand {
  GetComponentsCommand(uuid__ uuid) : TcpCommand(uuid){};
  std::string execute(Engine *e) override;
  boost::json::array
  get_components_short(const std::vector<IComponent *> &components);
  int undo() override;

private:
};