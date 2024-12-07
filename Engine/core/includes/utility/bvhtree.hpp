#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

struct Triangle {
  glm::vec3 v0;
  glm::vec3 v1;
  glm::vec3 v2;

  std::vector<Triangle> createCubeInObjectSpace() {
    std::vector<Triangle> cube;

    // Define the 8 vertices of a unit cube centered at the origin
    static glm::vec3 vertices[8] = {
        {-0.5f, -0.5f, -0.5f}, // 0: Bottom-left-back
        { 0.5f, -0.5f, -0.5f}, // 1: Bottom-right-back
        { 0.5f,  0.5f, -0.5f}, // 2: Top-right-back
        {-0.5f,  0.5f, -0.5f}, // 3: Top-left-back
        {-0.5f, -0.5f,  0.5f}, // 4: Bottom-left-front
        { 0.5f, -0.5f,  0.5f}, // 5: Bottom-right-front
        { 0.5f,  0.5f,  0.5f}, // 6: Top-right-front
        {-0.5f,  0.5f,  0.5f}  // 7: Top-left-front
    };

    // Define the 12 triangles (2 per face)
    // Back face
    cube.push_back({vertices[0], vertices[1], vertices[2]});
    cube.push_back({vertices[0], vertices[2], vertices[3]});

    // Front face
    cube.push_back({vertices[4], vertices[5], vertices[6]});
    cube.push_back({vertices[4], vertices[6], vertices[7]});

    // Left face
    cube.push_back({vertices[0], vertices[3], vertices[7]});
    cube.push_back({vertices[0], vertices[7], vertices[4]});

    // Right face
    cube.push_back({vertices[1], vertices[5], vertices[6]});
    cube.push_back({vertices[1], vertices[6], vertices[2]});

    // Bottom face
    cube.push_back({vertices[0], vertices[4], vertices[5]});
    cube.push_back({vertices[0], vertices[5], vertices[1]});

    // Top face
    cube.push_back({vertices[3], vertices[2], vertices[6]});
    cube.push_back({vertices[3], vertices[6], vertices[7]});

    return cube;
  }
};

struct BoundingBox {
  glm::vec3 min;
  glm::vec3 max;
};

struct BVHNode {
  BoundingBox box;
  int leftchild;
  int rightchild;
  int start;
  int count;
  bool isLeaf;

  BVHNode() : leftchild(-1), rightchild(-1), start(-1), isLeaf(false) {}
};



class BVH {
  std::vector<Triangle> triangles;
  std::vector<int> indices;
  int maxleafsize;
  std::unique_ptr<BVHNode> root = nullptr;


  BoundingBox computeBounding(int start, int end) {
    BoundingBox box;
    box.min = glm::vec3(std::numeric_limits<float>::infinity());
    box.max = glm::vec3(-std::numeric_limits<float>::infinity());

    for (int i = start; i < end; i++) {
      const Triangle &triforce = triangles[indices[i]];
      box.min = glm::min(box.min, triforce.v0);
      box.min = glm::min(box.min, triforce.v1);
      box.min = glm::min(box.min, triforce.v2);

      box.max = glm::min(box.max, triforce.v0);
      box.max = glm::min(box.max, triforce.v0);
      box.max = glm::min(box.max, triforce.v0);
    }

    return box;
  }

  std::string vec3ToString(const glm::vec3 &vec) {
    std::ostringstream oss;
    oss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
  }

  std::unique_ptr<BVHNode> buildNode(int start, int end) {
    int count = end - start;
    std::unique_ptr<BVHNode> node = std::make_unique<BVHNode>();
    node->box = computeBounding(start, end);

    std::cout << "\n Building Nodes for indices range [" << start << " , "
              << end << ") \n";
    std::cout << "\n Bounding Box Calculated | Min :"
              << vec3ToString(node->box.min)
              << " | Max :" << vec3ToString(node->box.max) << std::endl;

	return node;
  }

public:
  BVH(const std::vector<Triangle> &triforce, int maxLeafSize = 2)
      : triangles(triforce), maxleafsize(maxLeafSize) {
	std::cout << "in constructor\n";
    indices.resize(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++) {
      indices[i] = (int)i;
    }
	root = buildNode(0, (int)triangles.size());


  }
};


