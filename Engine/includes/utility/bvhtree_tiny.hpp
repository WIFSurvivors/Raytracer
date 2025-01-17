#pragma once
#include "glm/ext/matrix_transform.hpp"
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
#include "includes/utility/Log.hpp"
#include "includes/utility/data_loader.hpp"
using RT::Log;

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
  glm::vec3 normal;
  float pad0;
};

struct alignas(16) Materials {
  glm::vec3 Kd; // Diffuse Color
  float Ns;     // Specular Exponent
  glm::vec3 Ka; // Ambient Color
  float Ni;     // Optical Density
  glm::vec3 Ks; // Specular Color
  float d;      // Dissolve
  int illum;    // Illumination
  float pad0;
  float pad1;
  float pad2;
};

struct ObjectData {
  std::vector<Triangle> triangles;
  Materials material;
};

struct Vec3Hash {
  size_t operator()(const glm::vec3 &v) const {
    size_t h1 = std::hash<float>{}(v.x);
    size_t h2 = std::hash<float>{}(v.y);
    size_t h3 = std::hash<float>{}(v.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

struct TreeBuilder {

  tinybvh::BVH tree;
  std::vector<SSBONodes> ssboData;
  std::vector<Triangle> triangles;
  std::vector<Triangle> inserted_triangles;
  std::vector<uint32_t> triIdxData; // Triangle Indices. Stores Indices of
                                    // triangles triIdxData[5] = 32
  std::vector<Vec3Padded> vertex;   // Vertices vertex[32+0/1/2] = triangle

  std::vector<Materials> mats;
  std::vector<uint32_t> matIndx;

  std::unordered_map<glm::vec3, glm::vec3, Vec3Hash> vertexToNormal;
  int RenderEntities;
  std::vector<std::shared_ptr<MeshGallary>> gallary;

  TreeBuilder() {}

  int get_numberOfTriangles() { return triangles.size(); }

  // void update_gallary(MeshGallary &mesh_object) {
  //   bool found = false;
  //   for (auto &c : gallary) {
  //     if (mesh_object.id == c.id) {
  //       c._meshes.clear();
  //       for (RenderComponentMesh &mesh : mesh_object._meshes) {
  //         for (glm::vec3 &vertex : mesh._vertices) {
  //           vertex = glm::vec3(mesh_object.model * glm::vec4(vertex, 1.0f));
  //         }
  //       }

  //      c._meshes = mesh_object._meshes;
  //      found = true;
  //      break;
  //    }
  //  }

  //  if (!found) {

  //    for (RenderComponentMesh &mesh : mesh_object._meshes) {
  //      for (glm::vec3 &vertex : mesh._vertices) {
  //        vertex = glm::vec3(mesh_object.model * glm::vec4(vertex, 1.0f));
  //      }
  //    }

  //    gallary.push_back(mesh_object);
  //  }
  //}

  // void new_gallary(MeshGallary &mesh_object) {
  //   gallary.clear();
  //   for (RenderComponentMesh &mesh : mesh_object._meshes) {
  //     for (glm::vec3 &vertex : mesh._vertices) {
  //       vertex = glm::vec3(mesh_object.model * glm::vec4(vertex, 1.0f));
  //     }
  //     gallary.push_back(mesh_object);
  //   }
  // }

  void setGallary(std::vector<std::shared_ptr<MeshGallary>> o) { gallary = o; }
  void loadData() {

    if (gallary.empty()) {
      LOG_ERROR("Objects are not filled");
      return;
    }
    triangles.clear();
    mats.clear();
    matIndx.clear();
    vertex.clear();
    inserted_triangles.clear();
    ssboData.clear();
    vertexToNormal.clear();

    for (auto &obj : gallary) {
      for (auto &mesh : obj->_meshes) {

        // mats.push_back(Materials(mesh.MeshMaterial.Kd,0.0f,mesh.MeshMaterial.Ka,0.0f,mesh.MeshMaterial.Ks,0.0f,mesh.MeshMaterial.d,mesh.MeshMaterial.illum,mesh.MeshMaterial.Ns,mesh.MeshMaterial.Ni));

        mats.push_back(Materials(mesh.MeshMaterial.Kd, 0.05f,
                                 mesh.MeshMaterial.Ka, mesh.MeshMaterial.Ni,
                                 mesh.MeshMaterial.Ks, mesh.MeshMaterial.d,
                                 mesh.MeshMaterial.illum, 0.0f, 0.0f, 0.0f));
		
        std::cout << "NS: " << mesh.MeshMaterial.Ns << std::endl;
        for (int i = 0; i < mesh._indices.size(); i += 3) {

          glm::vec3 v0 = mesh._vertices[mesh._indices[i]];
          glm::vec3 v1 = mesh._vertices[mesh._indices[i + 1]];
          glm::vec3 v2 = mesh._vertices[mesh._indices[i + 2]];
          glm::vec3 n0 = mesh._normals[mesh._indices[i]];
          glm::vec3 n1 = mesh._normals[mesh._indices[i + 1]];
          glm::vec3 n2 = mesh._normals[mesh._indices[i + 2]];

          Triangle tri{v0, v1, v2};
          inserted_triangles.push_back(tri);
          matIndx.push_back(mats.size() - 1);

          vertexToNormal[v0] = n0;
          vertexToNormal[v1] = n1;
          vertexToNormal[v2] = n2;
        }
      }
    }

    std::vector<tinybvh::bvhvec4> bvhData =
        convertToBVHFormat(inserted_triangles);

    tree.Build(bvhData.data(), inserted_triangles.size());
    std::cout << "2\n";
    tree.Compact();

    triIdxData.assign(tree.triIdx, tree.triIdx + tree.triCount);
    if (tree.verts) { // Ensure verts is not null
      for (uint32_t i = 0; i < tree.triCount * 3; ++i) {
        const auto &v = tree.verts[i];
        glm::vec3 vert = glm::vec3(v.x, v.y, v.z);

        auto iterator = vertexToNormal.find(vert);
        if (iterator != vertexToNormal.end()) {

          vertex.push_back(Vec3Padded(vert, 0.0f, vertexToNormal[vert],
                                      0.0f)); // Convert to glm::vec3
        } else {
          std::cout << "OOps normal is gone\n";
          vertex.push_back(
              Vec3Padded(vert, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f));
        }
      }
    }

    std::cout << "TRIANGLES: " << inserted_triangles.size() << std::endl;

    std::vector<uint32_t> rearrangedMatIndx(tree.triCount);

    for (size_t i = 0; i < tree.triCount; ++i) {
      rearrangedMatIndx[i] = matIndx[triIdxData[i]];
    }

    matIndx = rearrangedMatIndx;
  }

  void loadData(std::vector<ObjectData> objects) {
    if (objects.empty()) {
      LOG_ERROR("Objects are not filled");
      return;
    }

    triangles.clear();
    mats.clear();
    matIndx.clear();
    vertex.clear();
    inserted_triangles.clear();
    ssboData.clear();

    for (auto &obj : objects) {
      inserted_triangles.insert(inserted_triangles.end(), obj.triangles.begin(),
                                obj.triangles.end());
    }

    std::vector<tinybvh::bvhvec4> bvhData =
        convertToBVHFormat(inserted_triangles);
    tree.Build(bvhData.data(), inserted_triangles.size());
    tree.Compact();
    triIdxData.assign(tree.triIdx, tree.triIdx + tree.triCount);

    if (tree.verts) { // Ensure verts is not null
      for (uint32_t i = 0; i < tree.triCount * 3; ++i) {
        const auto &v = tree.verts[i];
        vertex.push_back(
            Vec3Padded(glm::vec3(v.x, v.y, v.z), 0.0f)); // Convert to glm::vec3
      }
    }

    int matIndex = 0;
    for (auto &obj : objects) {
      mats.push_back(obj.material);
      for (size_t i = 0; i < obj.triangles.size(); ++i) {
        matIndx.push_back(matIndex);
      }
      matIndex += 1;
    }

    std::vector<uint32_t> rearrangedMatIndx(tree.triCount);

    for (size_t i = 0; i < tree.triCount; ++i) {
      rearrangedMatIndx[i] = matIndx[triIdxData[i]];
    }

    matIndx = rearrangedMatIndx;
  }

  void loadSampleData() {

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

    mats.push_back(Materials{glm::vec3(0.8f, 0.2f, 0.8f), 0.0f});
    mats.push_back(Materials{glm::vec3(0.0f, 0.0f, 1.0f), 0.0f});
    mats.push_back(Materials{glm::vec3(0.8f, 0.2f, 0.2f), 0.0f});
    mats.push_back(Materials{glm::vec3(0.1f, 0.6f, 0.5f), 0.0f});
    mats.push_back(Materials{glm::vec3(0.0f, 0.6f, 0.9f), 0.0f});

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
    // uint32_t nodeCount = tree.usedBVHNodes; // Number of nodes in the BVH
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

    /*for (uint32_t i = 0; i < tree.usedNodes; i++) {*/
    /**/
    /*  const auto &node = ssboData[i];*/
    /*  std::cout << "Node[" << i << "] : \n";*/
    /*  std::cout << "\t Left Child: " << node.leftchild << std::endl;*/
    /*  std::cout << "\t Right Child: " << node.rightchild << std::endl;*/
    /*  std::cout << "\t start: " << node.start << std::endl;*/
    /*  std::cout << "\t count: " << node.count << std::endl;*/
    /*}*/

    int count = 0;
    for (auto &tri : inserted_triangles) {
      std::cout << "Triangle : " << count << std::endl;
      std::cout << "\t v0(" << tri.v0.x << "," << tri.v0.y << "," << tri.v0.z
                << ")" << std::endl;
      std::cout << "\t v1(" << tri.v1.x << "," << tri.v1.y << "," << tri.v1.z
                << ")" << std::endl;
      std::cout << "\t v2(" << tri.v2.x << "," << tri.v2.y << "," << tri.v2.z
                << ")" << std::endl;
      count++;
    }
  }
};
