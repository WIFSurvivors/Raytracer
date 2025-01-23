#include "includes/utility/BigJsonReader.hpp"
#include "includes/utility/quicktype_json.hpp"

#include "includes/utility/Log.hpp"
#include "includes/AssetManager.hpp"

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <algorithm>
#include <format>

namespace util {
boost::uuids::uuid to_uuid(const std::string &s) {
  return boost::lexical_cast<boost::uuids::uuid>(s);
}

std::string to_string(boost::uuids::uuid id) {
  return boost::uuids::to_string(id);
}
} // namespace util

namespace RT {
std::string BigJson::readFileToString(const std::filesystem::path filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath.string());
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

void BigJson::write_string_to_path(const std::filesystem::path filePath,
                                   const std::string &content) {
  LOG(content);
  std::ofstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath.string());
  }

  file << content;

  if (!file) {
    throw std::ios_base::failure("Failed to write to file: " +
                                 filePath.string());
  }
}

bool BigJson::write_to_json(const std::filesystem::path filePath, Scene *s) {
  using quicktype::QuicktypeJson;

  auto path = std::filesystem::absolute(filePath);
  LOG(std::format("Try checking if file exist: {}", path.string()));
  if (std::filesystem::exists(path)) {
    LOG_WARN(std::format("File {} already exist. Content will be overwritten!",
                         path.string()));
  }

  QuicktypeJson qtjson{};

  {
    LOG("==CONVERTING METADATA==");
    auto rsys = s->get_render_system();
    quicktype::Metadata met{};
    met.set_max_depth(rsys->get_bounces());
    qtjson.set_metadata(met);
  }

  {
    LOG("==CONVERTING RESOURCES==");
    auto am = s->get_asset_manager();
    std::vector<quicktype::Resource> res{};
    for (auto &asset : am->get_storage()) {
      quicktype::Resource r{};
      r.set_path(asset.second.string());
      //   r.set_type("we don't have types :)");
      r.set_uuid(to_string(asset.first));
      res.push_back(r);
    }
    qtjson.set_resources(res);
  }

  {
    LOG("==CONVERTING ENTITY==");
    auto root = s->get_root().lock();
    std::vector<quicktype::Entity> entities;
    for (auto &ent : root->get_child_entities()) {
      quicktype::Entity qtent{};
      { // Name + UUID
        qtent.set_name(ent->get_name());
        qtent.set_uuid(util::to_string(ent->get_uuid()));
        LOG(std::format("-> Converted Entity to \"{}\" ({})",
                        qtent.get_name().value(), qtent.get_uuid().value()));
      }
      { // Translation
        quicktype::Translation qttrans{};

        quicktype::Position qtpos{};
        qtpos.set_x(ent->get_local_position().x);
        qtpos.set_y(ent->get_local_position().y);
        qtpos.set_z(ent->get_local_position().z);
        qttrans.set_position(qtpos);

        quicktype::Position qtrot{};
        qtrot.set_x(ent->get_local_rotation().x);
        qtrot.set_y(ent->get_local_rotation().y);
        qtrot.set_z(ent->get_local_rotation().z);
        qttrans.set_rotation(qtrot);

        quicktype::Position qtsca{};
        qtsca.set_x(ent->get_local_scale().x);
        qtsca.set_y(ent->get_local_scale().y);
        qtsca.set_z(ent->get_local_scale().z);
        qttrans.set_scale(qtsca);

        qtent.set_translation(qttrans);
      }
      { // Components
        quicktype::Components qtcomp{};
        auto um = s->get_uuid_manager();
        for (auto &comp_uuid : ent->get_components()) {
          auto sto = um->get_storage(comp_uuid);
          if (auto sys = dynamic_cast<RT::CameraSystem *>(sto)) {
            auto o_c = sys->get_component(comp_uuid);
            if (o_c.has_value(); auto c = o_c.value()) {
              quicktype::CameraComponent qtcc{};
              qtcc.set_fov(c->get_fov());
              // qtcc.set_aspect_ratio(); // we don't support this
              qtcc.set_near_clip(c->get_near());
              qtcc.set_far_clip(c->get_far());
              qtcomp.set_camera_component(qtcc);
            }
          } else if (auto sys = dynamic_cast<RT::LightSystem *>(sto)) {
            auto o_c = sys->get_component(comp_uuid);
            if (o_c.has_value(); auto c = o_c.value()) {
              quicktype::LightComponent qtlc{};

              quicktype::Color color{};
              color.set_r(c->get_color().r);
              color.set_g(c->get_color().g);
              color.set_b(c->get_color().b);
              qtlc.set_color(color);

              qtlc.set_intensity(c->get_intensity());

              qtcomp.set_light_component(qtlc);
            }
          } else if (auto sys = dynamic_cast<RT::RenderSystem *>(sto)) {
            auto o_c = sys->get_component(comp_uuid);
            if (o_c.has_value(); auto c = o_c.value()) {
              quicktype::RenderComponent qtrc{};
              qtrc.set_obj_uuid(util::to_string(c->get_obj_asset().get_uuid()));
              qtcomp.set_render_component(qtrc);
            }
          }
        }
        qtent.set_components(qtcomp);
      }
      entities.push_back(qtent);
    }
    qtjson.set_entity(entities);
  }

  nlohmann::json final_json{};
  quicktype::to_json(final_json, qtjson);
  std::string raw = final_json.dump(2); // prettify
  try {
    write_string_to_path(path, raw);
  } catch (std::exception &ex) {
    LOG_ERROR(std::format("Couldn't write json!", path.string()));
    return false;
  }
  return true;
}

std::optional<std::unique_ptr<Scene>>
BigJson::read_from_json(const std::filesystem::path filePath, Engine *engine) {
  using quicktype::QuicktypeJson;

  auto path = std::filesystem::absolute(filePath);
  LOG(std::format("Try opening .json file: {}", path.string()));
  if (!std::filesystem::exists(path)) {
    LOG_ERROR(std::format("File {} doesn't exist.", path.string()))
    return {};
  }

  std::unique_ptr<Scene> new_scene =
      std::make_unique<Scene>(engine, path.filename().string());

  LOG("==[START PARSING]==");
  auto str = readFileToString(path);
  QuicktypeJson data = nlohmann::json::parse(str);

  LOG("==PARSING METADATA==");
  auto metadata = data.get_metadata();
  if (metadata.has_value()) {
    // not settable
    // auto a = metadata->get_background_color();

    // looks bad
    // auto b = metadata->get_global_illumination();

    // not required due to deterministic ray tracing
    // auto d = metadata->get_samples_per_pixel();

    auto o_depth = metadata->get_max_depth(); // bounces
    if (o_depth.has_value()) {
      new_scene->get_render_system()->set_bounces(o_depth.value());
    }
  }

  auto asset_manager = new_scene->get_asset_manager();
  auto o_resources = data.get_resources();
  LOG("==PARSING RESOURCES==");
  if (o_resources.has_value()) {
    auto resources = o_resources.value();
    for (auto &&r : resources) {
      auto id = r.get_uuid();
      auto path = r.get_path();
      if (!id.has_value() || !path.has_value()) {
        LOG_ERROR(std::format(
            "Resource in JSON not correctly set. UUID or Path is missing!"));
        continue;
      }

      auto abs_path = std::filesystem::absolute(path.value());
      std::cout << "-- TRYING TO ACCESS PATH " << abs_path.string() << "\n";

      if (!std::filesystem::exists(abs_path)) {
        std::cerr << "!! FILE DOESN'T EXIST" << "\n";
        continue;
      }
      auto u = util::to_uuid(id.value());
      asset_manager->set(u, abs_path);
    }
  }

  LOG("==PARSING ENTITIES==");
  // if there is only one camera, it will automatically be deemed the main one
  bool has_found_camera = false;
  auto o_entities = data.get_entity();
  if (o_entities.has_value()) {
    auto entities = o_entities.value();
    for (auto &&e : entities) {
      auto name = e.get_name().has_value() ? e.get_name().value() : "undefined";
      auto has_uuid = e.get_uuid().has_value();

      LOG(std::format("PARSING TRANSLATION FOR {}", name));
      std::shared_ptr<Entity> new_entity = nullptr;
      if (!has_uuid) {
        new_entity = new_scene->create_entity(name);
      } else {
        auto u = util::to_uuid(e.get_uuid().value());
        new_entity = new_scene->create_entity(name, u);
      }

      auto has_translation = e.get_translation().has_value();
      if (has_translation) {
        auto translation = e.get_translation().value();
        if (translation.get_position().has_value()) {
          auto position = translation.get_position().value();

          float x = 0.f;
          if (position.get_x().has_value())
            x = position.get_x().value();

          float y = 0.f;
          if (position.get_y().has_value())
            y = position.get_y().value();

          float z = 0.f;
          if (position.get_z().has_value())
            z = position.get_z().value();

          new_entity->set_local_position(x, y, z);
        }

        if (translation.get_rotation().has_value()) {
          auto rotation = translation.get_rotation().value();

          float x = 0.f;
          if (rotation.get_x().has_value())
            x = rotation.get_x().value();

          float y = 0.f;
          if (rotation.get_y().has_value())
            y = rotation.get_y().value();

          float z = 0.f;
          if (rotation.get_z().has_value())
            z = rotation.get_z().value();

          new_entity->set_local_rotation(x, y, z);
        }

        if (translation.get_scale().has_value()) {
          auto scale = translation.get_scale().value();

          float x = 1.f;
          if (scale.get_x().has_value())
            x = scale.get_x().value();

          float y = 1.f;
          if (scale.get_y().has_value())
            y = scale.get_y().value();

          float z = 1.f;
          if (scale.get_z().has_value())
            z = scale.get_z().value();

          new_entity->set_local_scale(x, y, z);
        }
      }

      LOG(std::format("PARSING COMPONENTS FOR {}", name));
      auto has_components = e.get_components().has_value();
      if (has_components) {
        auto components = e.get_components().value();

        if (components.get_camera_component().has_value()) {
          LOG(std::format("=> CREATE CAMERA COMPONENT FOR {}", name));
          auto json_cc = components.get_camera_component().value();
          auto camera = new_scene->get_camera_system()->create_component(
              new_entity.get());

          if (json_cc.get_fov().has_value()) {
            camera->set_fov(json_cc.get_fov().value());
          }
          if (json_cc.get_far_clip().has_value()) {
            camera->set_far(json_cc.get_far_clip().value());
          }
          if (json_cc.get_near_clip().has_value()) {
            camera->set_near(json_cc.get_near_clip().value());
          }

          json_cc.get_aspect_ratio(); // that's weird...
        }
        if (components.get_light_component().has_value()) {
          LOG(std::format("=> CREATE LIGHT COMPONENT FOR {}", name));
          has_found_camera = true;
          auto json_lc = components.get_light_component().value();

          auto lc =
              new_scene->get_light_system()->create_component(new_entity.get());
          if (json_lc.get_intensity().has_value()) {
            lc->set_intensity(json_lc.get_intensity().value());
          }

          if (json_lc.get_color().has_value()) {
            auto color = json_lc.get_color().value();
            color.get_b().has_value();

            float r = 0.f;
            if (color.get_r().has_value())
              r = color.get_r().value();

            float g = 0.f;
            if (color.get_g().has_value())
              g = color.get_g().value();

            float b = 0.f;
            if (color.get_b().has_value())
              b = color.get_b().value();

            lc->set_color(r, g, b);
          }
        }

        if (components.get_render_component().has_value()) {
          LOG(std::format("=> CREATE RENDER COMPONENT FOR {}", name));
          auto json_rc = components.get_render_component().value();

          auto rc = new_scene->get_render_system()->create_component(
              new_entity.get());

          if (json_rc.get_obj_uuid().has_value()) {
            rc->set_obj_asset(util::to_uuid(json_rc.get_obj_uuid().value()));
          }
        }
      }
    }
  }

  if (!has_found_camera) {
    LOG("==CREATING DEFAULT COMPONENT==");
    auto e = new_scene->create_entity("Camera");
    e->set_local_position(0.f, 0.f, 10.f);
    auto c = new_scene->get_camera_system()->create_component(e.get());
  }

  LOG("==[PARSING DONE]==");
  LOG("Result ->");
  new_scene->print_system_data();
  return std::move(new_scene);
}
} // namespace RT
