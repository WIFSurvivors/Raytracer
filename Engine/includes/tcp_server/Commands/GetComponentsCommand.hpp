#pragma once
#include "includes/component/Component.hpp"
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/json.hpp>
#include <vector>

#define GET_COMPONENTS_COMMAND "GetComponents"

using RT::IComponent;

struct GetComponentsCommand : public TcpCommand {
  explicit GetComponentsCommand(uuid uuid) : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;
  boost::json::array get_components_short(RT::Scene *s, const std::vector<uuid> &components);

private:
};
