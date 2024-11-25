#pragma once

#include "includes/utility/SimpleLogger.hpp"
#include "includes/System.hpp"
#include "includes/SimpleSystem.hpp"
#include "includes/EntitySystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/RenderSystem.hpp"
#include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>

typedef boost::uuids::uuid uuid;
struct Entity;
class Engine;

struct Scene /*: public virtual System */ {
  Scene(Engine &e);

  std::weak_ptr<Entity> get_root();

  std::shared_ptr<Entity> create_entity(std::string name);
  std::shared_ptr<Entity> create_entity(std::string name, uuid id);
  std::shared_ptr<Entity> create_entity(std::string name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(std::string name, uuid id,
                                        std::shared_ptr<Entity> parent);

  void print();

  void generate_sample_content();

  UUIDManager *get_uuid_manager();

private:
  EntitySystem _entity_system{};
  SimpleSystem _simple_system{};

  UUIDManager _uuid_manager{};
  //   WindowManager _wm{};
  //   core::RenderSystem render_sys{&_wm};

  std::shared_ptr<Entity> _root;
};
