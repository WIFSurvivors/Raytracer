#pragma once

#include <memory>
#include <string>

struct entity;

static int entity_counter = 0;

struct scene {
  scene();
  ~scene();

  //   std::shared_ptr<entity> add_new_entity();
  //   bool remove_entity(std::shared_ptr<entity> e);

  std::weak_ptr<entity> get_root();

  std::shared_ptr<entity> create_entity(std::string name);
  std::shared_ptr<entity> create_entity(std::string name,
                                        std::shared_ptr<entity> parent);
  std::shared_ptr<entity>
  create_entity(std::string name, std::shared_ptr<entity> parent, long uuid);

  void print();

private:
  std::shared_ptr<entity> _root{};
};
