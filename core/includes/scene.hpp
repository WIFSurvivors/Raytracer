#pragma once

#include <memory>
#include <string>

struct entity;

static int64_t entity_counter = 0;

struct scene {
  scene();

  std::weak_ptr<entity> get_root();

  std::shared_ptr<entity> create_entity(std::string name);
  std::shared_ptr<entity> create_entity(std::string name, int64_t uuid);
  std::shared_ptr<entity> create_entity(std::string name,
                                        std::shared_ptr<entity> parent);
  std::shared_ptr<entity> create_entity(std::string name, int64_t uuid,
                                        std::shared_ptr<entity> parent);

  void print();

private:
  std::shared_ptr<entity> _root{};
};
