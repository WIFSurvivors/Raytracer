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
        {-1.0f, -1.0f, -1.0f}, // 0: Bottom-left-back
        {1.0f, -1.0f, -1.0f},  // 1: Bottom-right-back
        {1.0f, 1.0f, -1.0f},   // 2: Top-right-back
        {-1.0f, 1.0f, -1.0f},  // 3: Top-left-back
        {-1.0f, -1.0f, 1.0f},  // 4: Bottom-left-front
        {1.0f, -1.0f, 1.0f},   // 5: Bottom-right-front
        {1.0f, 1.0f, 1.0f},    // 6: Top-right-front
        {-1.0f, 1.0f, 1.0f}    // 7: Top-left-front
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

struct SSBOBVHNode {
  glm::vec3 bboxMin;
  glm::vec3 bboxMax;
  int leftChild;  // Left child index (-1 if leaf)
  int rightChild; // Right child index (-1 if leaf)
  int start;      // Start index of primitives (only for leaves)
  int count;      // Number of primitives (only for leaves)
  int isLeaf;     // 1 if leaf, 0 if internal
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
  std::shared_ptr<BVHNode> root = nullptr;
  std::unordered_map<
      std::shared_ptr<BVHNode>,
      std::pair<std::shared_ptr<BVHNode>, std::shared_ptr<BVHNode>>>
      children_map;
  std::unordered_map<std::shared_ptr<BVHNode>, int> nodeIndexMap;
  std::vector<std::shared_ptr<BVHNode>> nodeList;

  BoundingBox computeBounding(int start, int end) {
    BoundingBox box;
    box.min = glm::vec3(std::numeric_limits<float>::infinity());
    box.max = glm::vec3(-std::numeric_limits<float>::infinity());

    for (int i = start; i < end; i++) {
      const Triangle &triforce = triangles[indices[i]];
      box.min = glm::min(box.min, triforce.v0);
      box.min = glm::min(box.min, triforce.v1);
      box.min = glm::min(box.min, triforce.v2);

      box.max = glm::max(box.max, triforce.v0);
      box.max = glm::max(box.max, triforce.v1);
      box.max = glm::max(box.max, triforce.v2);
    }

    return box;
  }

  std::string vec3ToString(const glm::vec3 &vec) {
    std::ostringstream oss;
    oss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
  }

  float getComponent(const glm::vec3 &vicar_amelia, int axis) {
    if (axis < 0 || axis > 2) {
      std::cout << "Warning: Invalid axis " << axis
                << " requested. Defaulting to z.\n";
    }
    switch (axis) {
    case 0:
      return vicar_amelia.x;
    case 1:
      return vicar_amelia.y;
    default:
      return vicar_amelia.z;
    }
  }

  void createIndexMap(std::shared_ptr<BVHNode> node) {
    if (!node)
      return;
    if (nodeIndexMap.find(node) == nodeIndexMap.end()) {
      int newIndex = (int)nodeIndexMap.size();
      /*std::cout << "New Node Index: " << newIndex << std::endl;*/
      nodeIndexMap[node] = newIndex;
    }

    // Check if current node is leaf or not
    if (!node->isLeaf) {
      auto iterat = children_map.find(node);
      if (iterat != children_map.end()) {
        // first is key second is pair
        std::shared_ptr<BVHNode> leftChild = iterat->second.first;
        std::shared_ptr<BVHNode> rightChild = iterat->second.second;

        if (nodeIndexMap.find(leftChild) == nodeIndexMap.end()) {
          int newIndex = (int)nodeIndexMap.size();
          /*std::cout << "Left Child New Index: " << newIndex << std::endl;*/
          nodeIndexMap[leftChild] = newIndex;
        }
        if (nodeIndexMap.find(rightChild) == nodeIndexMap.end()) {
          int newIndex = (int)nodeIndexMap.size();
          /*std::cout << "Right Child New Index: " << newIndex << std::endl;*/
          nodeIndexMap[rightChild] = newIndex;
        }
        createIndexMap(leftChild);
        createIndexMap(rightChild);
      }
    }
  }

  glm::vec3 getCenter(const Triangle triforce) {
    return (triforce.v0 + triforce.v1 + triforce.v2) / 3.0f;
  }

  std::shared_ptr<BVHNode> buildNode(int start, int end) {
    // Amount of Primitive indexes
    int count = end - start;

    std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
    node->box = computeBounding(start, end);

    /*std::cout << "\n Building Nodes for indices range [" << start << " , "*/
    /*          << end << ") \n";*/
    /*std::cout << "\n Bounding Box Calculated | Min :"*/
    /*          << vec3ToString(node->box.min)*/
    /*          << " | Max :" << vec3ToString(node->box.max) << std::endl;*/

    if (count <= maxleafsize) {
      node->isLeaf = true;
      node->start = start;
      node->count = count;
      /*for (int i = start; i < end; i++) {*/
      /*  const Triangle &t = triangles[indices[i]];*/
      /*  std::cout << "Triangle " << i << ": " << vec3ToString(t.v0) << ", "*/
      /*            << vec3ToString(t.v1) << ", " << vec3ToString(t.v2) << "\n";*/
      /*}*/

      return node;
    }

    glm::vec3 axisDifference = {node->box.max.x - node->box.min.x,
                                node->box.max.y - node->box.min.y,
                                node->box.max.z - node->box.min.z};
    // max_element for finding the maximum of a given set e.g here axis
    // difference distance= calculate the position of the found max element:
    //  cppref: Return value: The number of increments needed to go from first
    //  to last. a = 0 | x a = 1 | y a = 2 | z
    int axis = std::distance(
        &axisDifference.x,
        std::max_element(&axisDifference.x, &axisDifference.z + 1));

    // split at midpoint not median
    //
    // root
    // ----------------------------------
    // |	t1			|				|
    // |				|	t4			|
    // |		b0		|		b1		|
    // |				|				|
    // |	t2		t3	|	t6		t7	|
    // |				|				|
    // ----------------------------------
    // dividing
    float midpoint = 0.5f * (getComponent(node->box.min, axis) +
                             getComponent(node->box.max, axis));
    // Sorting using std::partition ->  Tip provided by chatgpt how to sort
    int mid = std::partition(indices.begin() + start, indices.begin() + end,
                             [&](int i) {
                               const Triangle &t = triangles[i];
                               glm::vec3 center = getCenter(t);
                               float checkValue = getComponent(center, axis);
                               return checkValue < midpoint;
                             }) -
              indices.begin();

    if (mid == start | mid == end) {
      mid = start + count / 2;
    }

    std::shared_ptr<BVHNode> leftNode = buildNode(start, mid);
    std::shared_ptr<BVHNode> rightNode = buildNode(mid, end);

    nodeList.push_back(node);
    nodeList.push_back(leftNode);
    nodeList.push_back(rightNode);

    children_map[node] = std::make_pair(leftNode, rightNode);
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
  std::vector<std::shared_ptr<BVHNode>> flattenedNodes;

  void flatten() {
    flattenedNodes.clear();
    createIndexMap(root);
    flattenedNodes.resize(nodeIndexMap.size());
    for (auto &pair : nodeIndexMap) {
      std::shared_ptr<BVHNode> node = pair.first;
      int index = pair.second;
      flattenedNodes[index] = node;
      if (!node->isLeaf) {
        auto childs = children_map[node];
        flattenedNodes[index]->leftchild = nodeIndexMap[childs.first];
        flattenedNodes[index]->rightchild = nodeIndexMap[childs.second];
      }
    }
  }
  // Made by gpt
  void printFlattened() {
    std::cout << "\nFlattened BVH Nodes:\n";
    for (size_t i = 0; i < flattenedNodes.size(); i++) {
      std::shared_ptr<BVHNode> n = flattenedNodes[i];
      std::cout << "Node " << i << ": bbox[(" << n->box.min.x << ", "
                << n->box.min.y << ", " << n->box.min.z << "), ("
                << n->box.max.x << ", " << n->box.max.y << ", " << n->box.max.z
                << ")]";

      if (n->isLeaf) {
        std::cout << " Leaf with primitives: ";
        for (int j = 0; j < n->count; j++) {
          int primIndex = indices[n->start + j];
          std::cout << primIndex << " ";
        }
      } else {
        std::cout << " Internal node: left=" << n->leftchild
                  << " right=" << n->rightchild;
      }
      std::cout << "\n";
    }
  }

  std::vector<SSBOBVHNode> ssboData;

  void
  prepareSSBOData(const std::vector<std::shared_ptr<BVHNode>> &flattenedNodes) {
    for (const auto &n : flattenedNodes) {
      SSBOBVHNode node;
      node.bboxMin = n->box.min;
      node.bboxMax = n->box.max;
      node.leftChild = n->isLeaf ? -1 : n->leftchild;
      node.rightChild = n->isLeaf ? -1 : n->rightchild;
      node.start = n->start;
      node.count = n->count;
      node.isLeaf = n->isLeaf ? 1 : 0;
      ssboData.push_back(node);
    }
  }
};
