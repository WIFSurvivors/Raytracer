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

public:
  BVH(const std::vector<Triangle> &triforce, int maxLeafSize = 2)
      : triangles(triforce), maxleafsize(maxLeafSize) {

    indices.resize(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++) {
      indices[i] = (int)i;
    }
  }

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

private:
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
};
