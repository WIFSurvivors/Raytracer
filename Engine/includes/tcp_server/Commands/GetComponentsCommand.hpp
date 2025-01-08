#pragma once
#include "includes/component/Component.hpp"
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
#include <string>
#include <boost/json.hpp>
#include <vector>

#define GET_COMPONENTS_COMMAND "GetComponents"

struct IComponent;

struct GetComponentsCommand : public TcpCommand {
  explicit GetComponentsCommand(uuid uuid) : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;
  boost::json::array
  get_components_short(const std::vector<RT::IComponent *> &components);
  std::string undo() override;

private:
};
