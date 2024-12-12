#include <memory>
#define TINYBVH_IMPLEMENTATION
#include "tiny_bvh.h"
#ifdef _MSC_VER
#include "stdlib.h" // for rand
#include "stdio.h"  // for printf
#else
#include <cstdlib>
#include <cstdio>
#endif

#include <glm/vec3.hpp>

struct alignas(16) Triangle {
  glm::vec3 v0;
  float pad0; // Pad to 16 bytes
  glm::vec3 v1;
  float pad1; // Pad to 16 bytes
  glm::vec3 v2;
  float pad2; // Pad to 16 bytes

  Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2)
      : v0(v0), pad0(0.0f), v1(v1), pad1(0.0f), v2(v2), pad2(0.0f) {}
};

inline std::vector<Triangle> createCube(const glm::vec3 &origin) {
  return std::vector<Triangle>{// Front face
                               Triangle(glm::vec3(-1.0f, -1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin),
                               Triangle(glm::vec3(-1.0f, -1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, 1.0f) + origin),

                               // Back face
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, -1.0f) + origin),
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, -1.0f) + origin),

                               // Left face
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(-1.0f, -1.0f, 1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, 1.0f) + origin),
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, 1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, -1.0f) + origin),

                               // Right face
                               Triangle(glm::vec3(1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, 1.0f) + origin),
                               Triangle(glm::vec3(1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin),

                               // Top face
                               Triangle(glm::vec3(-1.0f, 1.0f, -1.0f) + origin,
                                        glm::vec3(-1.0f, 1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin),
                               Triangle(glm::vec3(-1.0f, 1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, 1.0f) + origin,
                                        glm::vec3(1.0f, 1.0f, -1.0f) + origin),

                               // Bottom face
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, 1.0f) + origin,
                                        glm::vec3(-1.0f, -1.0f, 1.0f) + origin),
                               Triangle(glm::vec3(-1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, -1.0f) + origin,
                                        glm::vec3(1.0f, -1.0f, 1.0f) + origin)};
}
inline std::vector<tinybvh::bvhvec4>
convertToBVHFormat(const std::vector<Triangle> &triangles) {
  std::vector<tinybvh::bvhvec4> bvhData;
  for (const auto &tri : triangles) {
    bvhData.emplace_back(tri.v0.x, tri.v0.y, tri.v0.z, 0.0f); // v0
    bvhData.emplace_back(tri.v1.x, tri.v1.y, tri.v1.z, 0.0f); // v1
    bvhData.emplace_back(tri.v2.x, tri.v2.y, tri.v2.z, 0.0f); // v2
  }
  return bvhData;
}

struct SSBONodes {
  glm::vec3 bboxMin;
  float pad0;
  glm::vec3 bboxMax;
  float pad1;
  int leftchild;
  int rightchild;
  int start;
  int count;
  int isLeaf;
  float pad2;
  float pad3;
  float pad4;
};

struct TreeBuilder {

  tinybvh::BVH tree;
  std::vector<SSBONodes> ssboData;
  std::vector<Triangle> triangles;

  TreeBuilder() {
    std::vector<Triangle> cube = createCube(glm::vec3{0.0f});
    std::vector<tinybvh::bvhvec4> bvhData = convertToBVHFormat(cube);

    tree.Build(bvhData.data(), cube.size());
    tree.Compact(tinybvh::BVH::WALD_32BYTE);
    tinybvh::BVH::BVHNode n;
  }

  void prepareSSBOData() {
    tinybvh::BVH::BVHNode *nodes =
        tree.bvhNode; // Pointer to the flattened BVH nodes
    uint32_t nodeCount = tree.usedBVHNodes; // Number of nodes in the BVH

    for (uint32_t i = 0; i < nodeCount; i++) {
      SSBONodes nodessbo;
      const auto &node = nodes[i];
      nodessbo.pad0 = 0.0f;
      nodessbo.pad1 = 0.0f;
      nodessbo.pad2 = 0.0f;
      nodessbo.pad3 = 0.0f;
      nodessbo.pad4 = 0.0f;

      nodessbo.bboxMax =
          glm::vec3{node.aabbMax.x, node.aabbMax.y, node.aabbMax.z};
      nodessbo.bboxMin =
          glm::vec3{node.aabbMin.x, node.aabbMin.y, node.aabbMin.z};

      if (node.isLeaf()) {
        nodessbo.leftchild = -1;
        nodessbo.rightchild = -1;
        nodessbo.count = node.triCount;
        nodessbo.start = node.leftFirst;
        nodessbo.isLeaf = 1;

        for (uint32_t j = 0; j < node.triCount; j++) {
          uint32_t index = tree.triIdx[node.leftFirst + j];
          uint32_t vIndex = index * 3;
          Triangle t = Triangle{
              glm::vec3(tree.verts[vIndex].x, tree.verts[vIndex].y,
                        tree.verts[vIndex].z),
              glm::vec3(tree.verts[vIndex + 1].x, tree.verts[vIndex + 1].y,
                        tree.verts[vIndex + 1].z),
              glm::vec3(tree.verts[vIndex + 2].x, tree.verts[vIndex + 2].y,
                        tree.verts[vIndex + 2].z)};
          triangles.push_back(t);
        }

      } else {
        nodessbo.leftchild = node.leftFirst;
        nodessbo.rightchild = node.leftFirst + 1;
        nodessbo.start = 0;
        nodessbo.count = 0;
        nodessbo.isLeaf = 0;
      }
      ssboData.push_back(nodessbo);
    }
  }

};
