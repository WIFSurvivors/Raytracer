#pragma once

#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Face {
    glm::vec3 _v0;
    glm::vec3 _v1;
    glm::vec3 _v2;
    glm::vec3 _n;
};

struct Material {
    glm::vec4 _col;
    float _dif;
    float _spec;
};

struct RenderComponent : Component {
  RenderComponent(uuid id, Entity *e);
  void init();
  void update();
  void destroy();

  int _nfaces = 0;

private:
    std::vector<Face> _faces;
};
