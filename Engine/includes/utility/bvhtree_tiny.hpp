#pragma once
#include <iostream>
#include <memory>
#define TINYBVH_IMPLEMENTATION
#include <tiny_bvh.h>
#ifdef _MSC_VER
#include <stdlib.h> // for rand
#include <stdio.h>  // for printf
#else
#include <cstdlib>
#include <cstdio>
#endif
#include <sstream>
#include <glm/vec3.hpp>
#include <vector>
#include <string>

struct alignas(16) Triangle {
  glm::vec3 v0;
  float pad0; // Pad to 16 bytes
  glm::vec3 v1;
  float pad1; // Pad to 16 bytes
  glm::vec3 v2;
  float pad2; // Pad to 16 bytes

  Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2)
      : v0(v0), pad0(0.0f), v1(v1), pad1(0.0f), v2(v2), pad2(0.0f) {}

  std::string toString() const {
    std::ostringstream oss;
    oss << "Triangle:\n";
    oss << "  v0: (" << v0.x << ", " << v0.y << ", " << v0.z << ")\n";
    oss << "  v1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")\n";
    oss << "  v2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")\n";
    return oss.str();
  }
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

struct Vec3Padded {
  glm::vec3 data;
  float pad;
};

struct alignas(16) Materials {
  glm::vec3 color;
  float reflection;
};

struct TreeBuilder {

  tinybvh::BVH tree;
  std::vector<SSBONodes> ssboData;
  std::vector<Triangle> triangles;
  std::vector<uint32_t> triIdxData;
  std::vector<Vec3Padded> vertex;
  std::vector<Materials> mats;

  std::vector<uint32_t> matIndx;

  TreeBuilder() {
    std::vector<Triangle> cube = createCube(glm::vec3{2.0f, 0.0f, 0.0f});

    std::vector<Triangle> triforce1 = createCube(glm::vec3{0.0f, -2.0f, 0.0f});
    std::vector<Triangle> triforce2 = createCube(glm::vec3{2.0f, 0.0f, 0.0f});
    std::vector<Triangle> triforce3 = createCube(glm::vec3{-2.0f, 0.0f, 0.0f});
    std::vector<Triangle> triforce4 = createCube(glm::vec3{0.0f, 0.0f, -2.0f});
    std::vector<Triangle> triforce5 = createCube(glm::vec3{0.0f, 2.0f, 0.0f});

    std::vector<Triangle> triforce = triforce1;
    triforce.insert(triforce.end(), triforce2.begin(), triforce2.end());
    triforce.insert(triforce.end(), triforce3.begin(), triforce3.end());
    triforce.insert(triforce.end(), triforce4.begin(), triforce4.end());
    triforce.insert(triforce.end(), triforce5.begin(), triforce5.end());

    std::vector<tinybvh::bvhvec4> bvhData = convertToBVHFormat(triforce);

    tree.Build(bvhData.data(), triforce.size());
    tree.Compact();
    triIdxData.assign(tree.triIdx, tree.triIdx + tree.triCount);

    if (tree.verts) { // Ensure verts is not null
      for (uint32_t i = 0; i < tree.triCount * 3; ++i) {
        const auto &v = tree.verts[i];
        vertex.push_back(
            Vec3Padded(glm::vec3(v.x, v.y, v.z), 0.0f)); // Convert to glm::vec3
      }
    }

    // Debug print vertices
    /*for (const auto &v : vertex) {
      std::cout << "Vertex: (" << v.data.x << ", " << v.data.y << ", "
                << v.data.z << ")" << std::endl;
    }*/

    mats.push_back(Materials{glm::vec3(0.8f, 0.2f, 0.8f),
                             0.2f}); // Light gray, slightly reflective
    mats.push_back(Materials{glm::vec3(0.2f, 0.6f, 0.1f),
                             0.3f}); // Sky blue, moderately reflective
    mats.push_back(Materials{glm::vec3(0.8f, 0.2f, 0.2f),
                             0.4f}); // Bright red, more reflective
    mats.push_back(Materials{glm::vec3(0.1f, 0.6f, 0.5f),
                             0.3f}); // Bright red, more reflective
    mats.push_back(Materials{glm::vec3(0.0f, 0.6f, 0.9f),
                             0.6f}); // Bright red, more reflective

    uint32_t materialIndex = 0;
    for (size_t i = 0; i < triforce1.size(); ++i) {
      matIndx.push_back(materialIndex);
    }
    materialIndex = 1;
    for (size_t i = 0; i < triforce2.size(); ++i) {
      matIndx.push_back(materialIndex);
    }
    materialIndex = 2;
    for (size_t i = 0; i < triforce3.size(); ++i) {
      matIndx.push_back(materialIndex);
    }

    materialIndex = 3;
    for (size_t i = 0; i < triforce4.size(); ++i) {
      matIndx.push_back(materialIndex);
    }

    materialIndex = 4;
    for (size_t i = 0; i < triforce5.size(); ++i) {
      matIndx.push_back(materialIndex);
    }

    std::vector<uint32_t> rearrangedMatIndx(tree.triCount);

    for (size_t i = 0; i < tree.triCount; ++i) {
      rearrangedMatIndx[i] = matIndx[triIdxData[i]];
    }

    matIndx = rearrangedMatIndx;
  }

  void prepareSSBOData() {
    tinybvh::BVH::BVHNode *nodes =
        tree.bvhNode; // Pointer to the flattened BVH nodes
    //uint32_t nodeCount = tree.usedBVHNodes; // Number of nodes in the BVH
	uint32_t nodeCount = tree.usedNodes;

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
              glm::vec3(vertex[vIndex].data.x, vertex[vIndex].data.y,
                        vertex[vIndex].data.z),
              glm::vec3(vertex[vIndex + 1].data.x, vertex[vIndex + 1].data.y,
                        vertex[vIndex + 1].data.z),
              glm::vec3(vertex[vIndex + 2].data.x, vertex[vIndex + 2].data.y,
                        vertex[vIndex + 2].data.z)};
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

  void checkData() {
    for (uint32_t i = 0; i < tree.idxCount; ++i) {
      std::cout << "triIdx[" << i << "] = " << tree.triIdx[i] << std::endl;
    }
    for (uint32_t i = 0; i < tree.triCount * 3; ++i) {
      std::cout << "vertex[" << i << "] = ( " << vertex[i].data.x << " , "
                << vertex[i].data.y << " , " << vertex[i].data.z << " ) \n";
    }

    for (uint32_t i = 0; i < tree.usedNodes; i++) {

      const auto &node = ssboData[i];
      std::cout << "Node[" << i << "] : \n";
      std::cout << "\t Left Child: " << node.leftchild << std::endl;
      std::cout << "\t Right Child: " << node.rightchild << std::endl;
      std::cout << "\t start: " << node.start << std::endl;
      std::cout << "\t count: " << node.count << std::endl;
    }
  }
};
