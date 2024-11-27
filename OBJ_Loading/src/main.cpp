#include <iostream>

#define TINYBVH_IMPLEMENTATION
#include "tiny_bvh.h"
#ifdef _MSC_VER
#include "stdlib.h"		// for rand
#include "stdio.h"		// for printf
#else
#include <cstdlib>
#include <cstdio>
#endif
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust triangulation.
// Requires C++11
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"
#include <glm/vec3.hpp>



int main() {
  
  tinybvh::bvhvec4 triangles[32*3];
  std::string inputfile = "../CornellBox-Original/CornellBox-Original.obj";
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "../CornellBox-Original/"; // Path to material files

  tinyobj::ObjReader reader;
  std::vector<glm:: vec3> vetexList;
  if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
      std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
  }

  if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
  }
  int count2=0;
  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
        
        glm::vec3 vertex = glm::vec3(vx,vy,vz);
        vetexList.push_back(vertex);
        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        if (idx.normal_index >= 0) {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
        }        
        
        // Check if `texcoord_index` is zero or positive. negative = no texcoord
        // data
        if (idx.texcoord_index >= 0) {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
        }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
      }
      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  for(int i = 0; i < vetexList.size()/3; i++){
    triangles[i*3 + 0 ] = vetexList[i*3].x;
    triangles[i*3 + 1 ] = vetexList[i*3].y;
    triangles[i*3 + 2 ] = vetexList[i*3].z;
  }

	tinybvh::bvhvec3 O( 0.5f, 0.5f, -1 );
	tinybvh::bvhvec3 D( 0.1f, 0, 2 );
	tinybvh::Ray ray( O, D );
  tinybvh::BVH bvh;
  bvh.Build(triangles, vetexList.size()/3);
  bvh.Convert( tinybvh::BVH::WALD_32BYTE, tinybvh::BVH::VERBOSE, true );
  
  bvh.Refit( tinybvh::BVH::VERBOSE );

  std::cout << "Nodes : " << bvh.NodeCount(tinybvh::BVH::WALD_32BYTE) << "  Node: " << (bvh.usedBVHNodes) <<std::endl;
  assert( bvh.bvhNode != 0);
  auto* leftnode = &bvh.bvhNode[0];
  auto* rightnode = &bvh.bvhNode[1];
  std::cout << "left : " << leftnode->aabbMax.x <<std::endl;
  std::cout << "right : " << rightnode->aabbMax.x <<std::endl;
  unsigned retVal =0, nodeIdx=0, stack[64],stackPtr=0;
  while(1){
    const auto& n = bvh.bvhNode[nodeIdx];
    std::cout << n.aabbMax.x << std::endl;
    std::cout << n.aabbMax.y << std::endl;
    std::cout << n.aabbMax.z << std::endl;

    std::cout << " " << std::endl;
    std::cout << n.aabbMin.x << std::endl;
    std::cout << n.aabbMin.y << std::endl;
    std::cout << n.aabbMin.z << std::endl;
    std::cout << "---" << std::endl;
    retVal++;
    if(n.isLeaf()) { if (stackPtr==0) break; else nodeIdx = stack[--stackPtr];}
    else nodeIdx = n.leftFirst, stack[stackPtr++] = n.leftFirst+1; 
  }
}

