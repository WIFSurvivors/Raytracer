#pragma once
#include "glm/ext/vector_float3.hpp"
#include <vector>
#include "includes/utility/obj_loader.hpp"
#include <boost/uuid/uuid.hpp>


struct RenderComponentMesh{
  std::vector<glm::vec3> _vertices;
  std::vector<unsigned int> _indices;
  objl::Material MeshMaterial;
  
};

struct MeshGallary{
  std::vector<RenderComponentMesh> _meshes;
  boost::uuids::uuid id;
  glm::mat4 model;
};

