//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     QuicktypeJson data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <regex>
#include <iostream>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
template <typename T> struct adl_serializer<std::shared_ptr<T>> {
  static void to_json(json &j, const std::shared_ptr<T> &opt) {
    if (!opt)
      j = nullptr;
    else
      j = *opt;
  }

  static std::shared_ptr<T> from_json(const json &j) {
    if (j.is_null())
      return std::make_shared<T>();
    else
      return std::make_shared<T>(j.get<T>());
  }
};
template <typename T> struct adl_serializer<std::optional<T>> {
  static void to_json(json &j, const std::optional<T> &opt) {
    if (!opt)
      j = nullptr;
    else
      j = *opt;
  }

  static std::optional<T> from_json(const json &j) {
    if (j.is_null())
      return std::make_optional<T>();
    else
      return std::make_optional<T>(j.get<T>());
  }
};
} // namespace nlohmann
#endif

namespace quicktype {
using nlohmann::json;

#ifndef NLOHMANN_UNTYPED_quicktype_HELPER
#define NLOHMANN_UNTYPED_quicktype_HELPER
inline json get_untyped(const json &j, const char *property) {
  if (j.find(property) != j.end()) {
    return j.at(property).get<json>();
  }
  return json();
}

inline json get_untyped(const json &j, std::string property) {
  return get_untyped(j, property.data());
}
#endif

#ifndef NLOHMANN_OPTIONAL_quicktype_HELPER
#define NLOHMANN_OPTIONAL_quicktype_HELPER
template <typename T>
inline std::shared_ptr<T> get_heap_optional(const json &j,
                                            const char *property) {
  auto it = j.find(property);
  if (it != j.end() && !it->is_null()) {
    return j.at(property).get<std::shared_ptr<T>>();
  }
  return std::shared_ptr<T>();
}

template <typename T>
inline std::shared_ptr<T> get_heap_optional(const json &j,
                                            std::string property) {
  return get_heap_optional<T>(j, property.data());
}
template <typename T>
inline std::optional<T> get_stack_optional(const json &j,
                                           const char *property) {
  auto it = j.find(property);
  if (it != j.end() && !it->is_null()) {
    return j.at(property).get<std::optional<T>>();
  }
  return std::optional<T>();
}

template <typename T>
inline std::optional<T> get_stack_optional(const json &j,
                                           std::string property) {
  return get_stack_optional<T>(j, property.data());
}
#endif

class CameraComponent {
public:
  CameraComponent() = default;
  virtual ~CameraComponent() = default;

private:
  std::optional<int64_t> fov;
  std::optional<double> aspect_ratio;
  std::optional<double> near_clip;
  std::optional<int64_t> far_clip;

public:
  std::optional<int64_t> get_fov() const { return fov; }
  void set_fov(std::optional<int64_t> value) { this->fov = value; }

  std::optional<double> get_aspect_ratio() const { return aspect_ratio; }
  void set_aspect_ratio(std::optional<double> value) {
    this->aspect_ratio = value;
  }

  std::optional<double> get_near_clip() const { return near_clip; }
  void set_near_clip(std::optional<double> value) { this->near_clip = value; }

  std::optional<int64_t> get_far_clip() const { return far_clip; }
  void set_far_clip(std::optional<int64_t> value) { this->far_clip = value; }
};

class Color {
public:
  Color() = default;
  virtual ~Color() = default;

private:
  std::optional<int64_t> r;
  std::optional<int64_t> g;
  std::optional<int64_t> b;

public:
  std::optional<int64_t> get_r() const { return r; }
  void set_r(std::optional<int64_t> value) { this->r = value; }

  std::optional<int64_t> get_g() const { return g; }
  void set_g(std::optional<int64_t> value) { this->g = value; }

  std::optional<int64_t> get_b() const { return b; }
  void set_b(std::optional<int64_t> value) { this->b = value; }
};

class LightComponent {
public:
  LightComponent() = default;
  virtual ~LightComponent() = default;

private:
  std::optional<double> intensity;
  std::optional<Color> color;

public:
  std::optional<double> get_intensity() const { return intensity; }
  void set_intensity(std::optional<double> value) { this->intensity = value; }

  std::optional<Color> get_color() const { return color; }
  void set_color(std::optional<Color> value) { this->color = value; }
};

class RenderComponent {
public:
  RenderComponent() = default;
  virtual ~RenderComponent() = default;

private:
  std::optional<std::string> obj_uuid;
  std::optional<std::string> mat_uuid;

public:
  std::optional<std::string> get_obj_uuid() const { return obj_uuid; }
  void set_obj_uuid(std::optional<std::string> value) {
    this->obj_uuid = value;
  }

  std::optional<std::string> get_mat_uuid() const { return mat_uuid; }
  void set_mat_uuid(std::optional<std::string> value) {
    this->mat_uuid = value;
  }
};

class Components {
public:
  Components() = default;
  virtual ~Components() = default;

private:
  std::optional<RenderComponent> render_component;
  std::optional<LightComponent> light_component;
  std::optional<CameraComponent> camera_component;

public:
  std::optional<RenderComponent> get_render_component() const {
    return render_component;
  }
  void set_render_component(std::optional<RenderComponent> value) {
    this->render_component = value;
  }

  std::optional<LightComponent> get_light_component() const {
    return light_component;
  }
  void set_light_component(std::optional<LightComponent> value) {
    this->light_component = value;
  }

  std::optional<CameraComponent> get_camera_component() const {
    return camera_component;
  }
  void set_camera_component(std::optional<CameraComponent> value) {
    this->camera_component = value;
  }
};

class Position {
public:
  Position() = default;
  virtual ~Position() = default;

private:
  std::optional<double> x;
  std::optional<double> y;
  std::optional<double> z;

public:
  std::optional<double> get_x() const { return x; }
  void set_x(std::optional<double> value) { this->x = value; }

  std::optional<double> get_y() const { return y; }
  void set_y(std::optional<double> value) { this->y = value; }

  std::optional<double> get_z() const { return z; }
  void set_z(std::optional<double> value) { this->z = value; }
};

class Translation {
public:
  Translation() = default;
  virtual ~Translation() = default;

private:
  std::optional<Position> position;
  std::optional<Position> rotation;
  std::optional<Position> scale;

public:
  std::optional<Position> get_position() const { return position; }
  void set_position(std::optional<Position> value) { this->position = value; }

  std::optional<Position> get_rotation() const { return rotation; }
  void set_rotation(std::optional<Position> value) { this->rotation = value; }

  std::optional<Position> get_scale() const { return scale; }
  void set_scale(std::optional<Position> value) { this->scale = value; }
};

class Entity {
public:
  Entity() = default;
  virtual ~Entity() = default;

private:
  std::optional<std::string> name;
  std::optional<std::string> uuid;
  std::optional<Translation> translation;
  std::optional<Components> components;

public:
  std::optional<std::string> get_name() const { return name; }
  void set_name(std::optional<std::string> value) { this->name = value; }

  std::optional<std::string> get_uuid() const { return uuid; }
  void set_uuid(std::optional<std::string> value) { this->uuid = value; }

  std::optional<Translation> get_translation() const { return translation; }
  void set_translation(std::optional<Translation> value) {
    this->translation = value;
  }

  std::optional<Components> get_components() const { return components; }
  void set_components(std::optional<Components> value) {
    this->components = value;
  }
};

class Metadata {
public:
  Metadata() = default;
  virtual ~Metadata() = default;

private:
  std::optional<std::vector<double>> background_color;
  std::optional<bool> global_illumination;
  std::optional<std::string> render_mode;
  std::optional<int64_t> max_depth;
  std::optional<int64_t> samples_per_pixel;

public:
  std::optional<std::vector<double>> get_background_color() const {
    return background_color;
  }
  void set_background_color(std::optional<std::vector<double>> value) {
    this->background_color = value;
  }

  std::optional<bool> get_global_illumination() const {
    return global_illumination;
  }
  void set_global_illumination(std::optional<bool> value) {
    this->global_illumination = value;
  }

  std::optional<std::string> get_render_mode() const { return render_mode; }
  void set_render_mode(std::optional<std::string> value) {
    this->render_mode = value;
  }

  std::optional<int64_t> get_max_depth() const { return max_depth; }
  void set_max_depth(std::optional<int64_t> value) { this->max_depth = value; }

  std::optional<int64_t> get_samples_per_pixel() const {
    return samples_per_pixel;
  }
  void set_samples_per_pixel(std::optional<int64_t> value) {
    this->samples_per_pixel = value;
  }
};

class Resource {
public:
  Resource() = default;
  virtual ~Resource() = default;

private:
  std::optional<std::string> uuid;
  std::optional<std::string> type;
  std::optional<std::string> path;

public:
  std::optional<std::string> get_uuid() const { return uuid; }
  void set_uuid(std::optional<std::string> value) { this->uuid = value; }

  std::optional<std::string> get_type() const { return type; }
  void set_type(std::optional<std::string> value) { this->type = value; }

  std::optional<std::string> get_path() const { return path; }
  void set_path(std::optional<std::string> value) { this->path = value; }
};

class QuicktypeJson {
public:
  QuicktypeJson() = default;
  virtual ~QuicktypeJson() = default;

private:
  std::optional<std::vector<Entity>> entity;
  std::optional<std::vector<Resource>> resources;
  std::optional<Metadata> metadata;

public:
  std::optional<std::vector<Entity>> get_entity() const { return entity; }
  void set_entity(std::optional<std::vector<Entity>> value) {
    this->entity = value;
  }

  std::optional<std::vector<Resource>> get_resources() const {
    return resources;
  }
  void set_resources(std::optional<std::vector<Resource>> value) {
    this->resources = value;
  }

  std::optional<Metadata> get_metadata() const { return metadata; }
  void set_metadata(std::optional<Metadata> value) { this->metadata = value; }
};
} // namespace quicktype

namespace quicktype {
using namespace nlohmann;
void from_json(const json &j, CameraComponent &x);
void to_json(json &j, const CameraComponent &x);

void from_json(const json &j, Color &x);
void to_json(json &j, const Color &x);

void from_json(const json &j, LightComponent &x);
void to_json(json &j, const LightComponent &x);

void from_json(const json &j, RenderComponent &x);
void to_json(json &j, const RenderComponent &x);

void from_json(const json &j, Components &x);
void to_json(json &j, const Components &x);

void from_json(const json &j, Position &x);
void to_json(json &j, const Position &x);

void from_json(const json &j, Translation &x);
void to_json(json &j, const Translation &x);

void from_json(const json &j, Entity &x);
void to_json(json &j, const Entity &x);

void from_json(const json &j, Metadata &x);
void to_json(json &j, const Metadata &x);

void from_json(const json &j, Resource &x);
void to_json(json &j, const Resource &x);

void from_json(const json &j, QuicktypeJson &x);
void to_json(json &j, const QuicktypeJson &x);

inline void from_json(const json &j, CameraComponent &x) {
  x.set_fov(get_stack_optional<int64_t>(j, "fov"));
  x.set_aspect_ratio(get_stack_optional<double>(j, "aspectRatio"));
  x.set_near_clip(get_stack_optional<double>(j, "nearClip"));
  x.set_far_clip(get_stack_optional<int64_t>(j, "farClip"));
}

inline void to_json(json &j, const CameraComponent &x) {
  j = json::object();
  if (x.get_fov()) {
    j["fov"] = x.get_fov();
  }
  if (x.get_aspect_ratio()) {
    j["aspectRatio"] = x.get_aspect_ratio();
  }
  if (x.get_near_clip()) {
    j["nearClip"] = x.get_near_clip();
  }
  if (x.get_far_clip()) {
    j["farClip"] = x.get_far_clip();
  }
}

inline void from_json(const json &j, Color &x) {
  x.set_r(get_stack_optional<int64_t>(j, "r"));
  x.set_g(get_stack_optional<int64_t>(j, "g"));
  x.set_b(get_stack_optional<int64_t>(j, "b"));
}

inline void to_json(json &j, const Color &x) {
  j = json::object();
  if (x.get_r()) {
    j["r"] = x.get_r();
  }
  if (x.get_g()) {
    j["g"] = x.get_g();
  }
  if (x.get_b()) {
    j["b"] = x.get_b();
  }
}

inline void from_json(const json &j, LightComponent &x) {
  x.set_intensity(get_stack_optional<double>(j, "intensity"));
  x.set_color(get_stack_optional<Color>(j, "color"));
}

inline void to_json(json &j, const LightComponent &x) {
  j = json::object();
  if (x.get_intensity()) {
    j["intensity"] = x.get_intensity();
  }
  if (x.get_color()) {
    j["color"] = x.get_color();
  }
}

inline void from_json(const json &j, RenderComponent &x) {
  x.set_obj_uuid(get_stack_optional<std::string>(j, "objUUID"));
  x.set_mat_uuid(get_stack_optional<std::string>(j, "matUUID"));
}

inline void to_json(json &j, const RenderComponent &x) {
  j = json::object();
  if (x.get_obj_uuid()) {
    j["objUUID"] = x.get_obj_uuid();
  }
  if (x.get_mat_uuid()) {
    j["matUUID"] = x.get_mat_uuid();
  }
}

inline void from_json(const json &j, Components &x) {
  x.set_render_component(
      get_stack_optional<RenderComponent>(j, "RenderComponent"));
  x.set_light_component(
      get_stack_optional<LightComponent>(j, "LightComponent"));
  x.set_camera_component(
      get_stack_optional<CameraComponent>(j, "CameraComponent"));
}

inline void to_json(json &j, const Components &x) {
  j = json::object();
  if (x.get_render_component()) {
    j["RenderComponent"] = x.get_render_component();
  }
  if (x.get_light_component()) {
    j["LightComponent"] = x.get_light_component();
  }
  if (x.get_camera_component()) {
    j["CameraComponent"] = x.get_camera_component();
  }
}

inline void from_json(const json &j, Position &x) {
  x.set_x(get_stack_optional<double>(j, "x"));
  x.set_y(get_stack_optional<int64_t>(j, "y"));
  x.set_z(get_stack_optional<int64_t>(j, "z"));
}

inline void to_json(json &j, const Position &x) {
  j = json::object();
  if (x.get_x()) {
    j["x"] = x.get_x();
  }
  if (x.get_y()) {
    j["y"] = x.get_y();
  }
  if (x.get_z()) {
    j["z"] = x.get_z();
  }
}

inline void from_json(const json &j, Translation &x) {
  x.set_position(get_stack_optional<Position>(j, "position"));
  x.set_rotation(get_stack_optional<Position>(j, "rotation"));
  x.set_scale(get_stack_optional<Position>(j, "scale"));
}

inline void to_json(json &j, const Translation &x) {
  j = json::object();
  if (x.get_position()) {
    j["position"] = x.get_position();
  }
  if (x.get_rotation()) {
    j["rotation"] = x.get_rotation();
  }
  if (x.get_scale()) {
    j["scale"] = x.get_scale();
  }
}

inline void from_json(const json &j, Entity &x) {
  x.set_name(get_stack_optional<std::string>(j, "name"));
  x.set_uuid(get_stack_optional<std::string>(j, "uuid"));
  x.set_translation(get_stack_optional<Translation>(j, "Translation"));
  x.set_components(get_stack_optional<Components>(j, "components"));
}

inline void to_json(json &j, const Entity &x) {
  j = json::object();
  if (x.get_name()) {
    j["name"] = x.get_name();
  }
  if (x.get_uuid()) {
    j["uuid"] = x.get_uuid();
  }
  if (x.get_translation()) {
    j["Translation"] = x.get_translation();
  }
  if (x.get_components()) {
    j["components"] = x.get_components();
  }
}

inline void from_json(const json &j, Metadata &x) {
  x.set_background_color(
      get_stack_optional<std::vector<double>>(j, "backgroundColor"));
  x.set_global_illumination(get_stack_optional<bool>(j, "globalIllumination"));
  x.set_render_mode(get_stack_optional<std::string>(j, "renderMode"));
  x.set_max_depth(get_stack_optional<int64_t>(j, "maxDepth"));
  x.set_samples_per_pixel(get_stack_optional<int64_t>(j, "samplesPerPixel"));
}

inline void to_json(json &j, const Metadata &x) {
  j = json::object();
  if (x.get_background_color()) {
    j["backgroundColor"] = x.get_background_color();
  }
  if (x.get_global_illumination()) {
    j["globalIllumination"] = x.get_global_illumination();
  }
  if (x.get_render_mode()) {
    j["renderMode"] = x.get_render_mode();
  }
  if (x.get_max_depth()) {
    j["maxDepth"] = x.get_max_depth();
  }
  if (x.get_samples_per_pixel()) {
    j["samplesPerPixel"] = x.get_samples_per_pixel();
  }
}

inline void from_json(const json &j, Resource &x) {
  x.set_uuid(get_stack_optional<std::string>(j, "uuid"));
  x.set_type(get_stack_optional<std::string>(j, "type"));
  x.set_path(get_stack_optional<std::string>(j, "path"));
}

inline void to_json(json &j, const Resource &x) {
  j = json::object();
  if (x.get_uuid()) {
    j["uuid"] = x.get_uuid();
  }
  if (x.get_type()) {
    j["type"] = x.get_type();
  }
  if (x.get_path()) {
    j["path"] = x.get_path();
  }
}

inline void from_json(const json &j, QuicktypeJson &x) {
  std::cout << "niap!" << std::endl;
  x.set_entity(get_stack_optional<std::vector<Entity>>(j, "entity"));
  x.set_resources(get_stack_optional<std::vector<Resource>>(j, "resources"));
  x.set_metadata(get_stack_optional<Metadata>(j, "metadata"));
}

inline void to_json(json &j, const QuicktypeJson &x) {
  std::cout << "pain!" << std::endl;
  j = json::object();
  if (x.get_entity()) {
    j["entity"] = x.get_entity();
  }
  if (x.get_resources()) {
    j["resources"] = x.get_resources();
  }
  if (x.get_metadata()) {
    j["metadata"] = x.get_metadata();
  }
}
} // namespace quicktype
