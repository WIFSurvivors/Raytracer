#include "includes/utility/BigJsonReader.hpp"
#include "includes/utility/quicktype_json.hpp"

#include "includes/utility/Log.hpp"
#include "includes/AssetManager.hpp"

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

boost::uuids::uuid to_uuid(const std::string &s) {
  return boost::lexical_cast<boost::uuids::uuid>(s);
}

namespace RT {
std::string readFileToString(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath);
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

void writeStringToFile(const std::string &filePath,
                       const std::string &content) {
  std::ofstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath);
  }

  file << content;

  if (!file) {
    throw std::ios_base::failure("Failed to write to file: " + filePath);
  }
}

void BigJsonReader::write_to_json(const std::filesystem::path filePath,
                                  std::unique_ptr<Scene>) {
  // awawwa
}

std::optional<std::unique_ptr<Scene>>
BigJsonReader::read_from_json(const std::filesystem::path filePath,
                              Engine *engine) {
  using namespace quicktype;
  auto test = std::filesystem::path(get_relative_json_folder_path() /
                                    "JsonParser_DummyFile.json");
  if (!std::filesystem::exists(test)) {
    LOG_WARN(std::format(
        "{} doesn't exist. Ignore this right now if you are not Jeb! :3",
        test.string()))
    return nullptr;
  }

  std::unique_ptr<Scene> new_scene =
      std::make_unique<Scene>(engine, filePath.filename());

  auto path = std::filesystem::absolute(test).string();
  std::cout << "PATH FOR JSON -> " << path << std::endl;
  auto str = readFileToString(path);
  // std::cout << str << std::endl;
  // writeStringToFile(path, str);
  QuicktypeJson data = nlohmann::json::parse(str);

  auto metadata = data.get_metadata();
  if (metadata.has_value()) {
    auto a = metadata->get_background_color();
    auto b = metadata->get_global_illumination();
    auto c = metadata->get_max_depth();
    auto d = metadata->get_samples_per_pixel();
  }

  auto asset_manager = new_scene->get_asset_manager();
  auto o_resources = data.get_resources();
  std::cout << "XXX " << o_resources.has_value() << "\n";

  if (o_resources.has_value()) {
    std::allocator<Resource> t;
    auto resources = o_resources.value();
    for (auto &&r : resources) {
      auto id = r.get_uuid();
      auto path = r.get_path();
      if (!id.has_value() || !path.has_value()) {
        LOG_ERROR(std::format(
            "Resource in JSON not correctly set. UUID or Path is missing!"));
        continue;
      }

      auto u = to_uuid(id.value());

      std::cout << "XXX Resource! " << path.value() << "\n";
      asset_manager->set(u, path.value());
    }
  }

  auto has_components = [](quicktype::Entity &e) {
    size_t component_count = 0;
    auto c = e.get_components().value();
    if (c.get_camera_component().has_value())
      component_count++;
    if (c.get_light_component().has_value())
      component_count++;
    if (c.get_render_component().has_value())
      component_count++;
    return component_count != 0;
  };

  // if there is only one camera, it will automatically be deemed the main one
  bool has_found_camera = false;
  auto o_entities = data.get_entity();
  if (o_entities.has_value()) {
    auto entities = o_entities.value();
    for (auto &&e : entities) {
      auto name = e.get_name().has_value() ? e.get_name().value() : "undefined";
      auto has_uuid = e.get_uuid().has_value();

      std::shared_ptr<Entity> t;
      if (has_uuid) {
        t = new_scene->create_entity(name);
      } else {
        auto u = to_uuid(e.get_uuid().value());
        t = new_scene->create_entity(name, u);
      }

      auto new_entity = new_scene->create_entity(name);
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

          float x = 0.f;
          if (scale.get_x().has_value())
            x = scale.get_x().value();

          float y = 0.f;
          if (scale.get_y().has_value())
            y = scale.get_y().value();

          float z = 0.f;
          if (scale.get_z().has_value())
            z = scale.get_z().value();

          new_entity->set_local_scale(x, y, z);
        }
      }

      auto has_components = e.get_components().has_value();
      if (has_components) {
        auto components = e.get_components().value();

        if (components.get_camera_component().has_value()) {
          auto json_cc = components.get_camera_component().value();
          auto camera = new_scene->get_camera_system()->create_component(
              new_entity.get());

          json_cc.get_fov();
          json_cc.get_near_clip();
          json_cc.get_aspect_ratio();
          json_cc.get_far_clip();
        }
        if (components.get_light_component().has_value()) {
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
          auto json_rc = components.get_render_component().value();

          auto rc = new_scene->get_render_system()->create_component(
              new_entity.get());

          if (json_rc.get_obj_uuid().has_value()) {
            // LOG_WARN(std::format(
            //     ".obj UUID {} found but not applied to render component",
            //     json_rc.get_obj_uuid().value()))
            rc->set_obj_asset(to_uuid(json_rc.get_mat_uuid().value()));
          }
        }
      }
    }
  }

  if (!has_found_camera) {
    auto e = new_scene->create_entity("Camera");
    e->set_local_position(0.f, 0.f, 10.f);
    auto c = new_scene->get_camera_system()->create_component(e.get());
  }

  std::cout << "\n";
  std::cout << "=========================================================\n";
  new_scene->print_system_data();
  std::cout << "=========================================================\n";
  std::cout << "\n";
  return std::move(new_scene);
}
} // namespace RT
