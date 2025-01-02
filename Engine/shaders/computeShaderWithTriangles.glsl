#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

// Model does not really exist rn so bleh
uniform float time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 cameraPos;

const int emitterCount_max = 10;
uniform int ls_active_light_sources;
uniform vec3[emitterCount_max] ls_positions;
uniform vec3[emitterCount_max] ls_directions; // not really required because we only want spherical light sources :/ 
uniform vec3[emitterCount_max] ls_colors;
uniform float[emitterCount_max] ls_intensities;

/*********************************************************************************/
//STRUCTS

// struct BVHNode {
//     vec3 bboxMin; // Minimum of bounding box
//     vec3 bboxMax; // Maximum of bounding box
//     int leftChild; // Index of the left child (-1 if leaf)
//     int rightChild; // Index of the right child (-1 if leaf)
//     int start; // Start index of primitives (leaf only)
//     int count; // Number of primitives (leaf only)
//     int isLeaf; // 1 if leaf, 0 if internal
// };

struct BVHNode {
    vec3 bboxMin; // Minimum of bounding box
    float pad0; // Padding for alignment
    vec3 bboxMax; // Maximum of bounding box
    float pad1; // Padding for alignment
    int leftChild; // Index of the left child (-1 if leaf)
    int rightChild; // Index of the right child (-1 if leaf)
    int start; // Start index of primitives (leaf only)
    int count; // Number of primitives (leaf only)
    int isLeaf; // 1 if leaf, 0 if internal
    int pad2; // Padding to align struct size to a multiple of 16 bytes
    int pad3; // Padding to align struct size to a multiple of 16 bytes
    int pad4; // Padding to align struct size to a multiple of 16 bytes
};

layout(std430, binding = 1) buffer BVHSSBO {
    BVHNode nodes[]; // Flattened BVH nodes
};

struct Triangle {
    vec3 v0;
    vec3 v1;
    vec3 v2;
};


struct Material{
  vec3 color; 
  float reflection;
};


layout(std430, binding = 5) buffer MaterialBuffer {
    Material materials[];
};

layout(std430, binding = 3) buffer TriangleSSBO {
    Triangle trianglesBuffer[];
};

layout(std430, binding = 4) buffer VertexBuffer {
    vec3 trivertex[];
};

layout(std430, binding = 2) buffer TriIndexBuffer {
    int triIdx[];
};

layout(std430, binding = 6) buffer MatIndexBuffer {
    int matIndex[];
};

struct Ray {
    vec3 origin;
    vec3 direction;
    int depth;
};

struct Light {
    vec3 position; // Position for point light
    vec3 direction; // Direction for directional light
    vec3 color; // Light color (RGB)
    float intensity; // Light intensity
};

/*********************************************************************************/
// VARIABLES
const int hittableCount = 60;
const int MAX_RECURSION_DEPTH = 4;

const int STACK_SIZE = 128;
Ray stack[STACK_SIZE];
int stackTop = 0;

const int BVH_STACK_SIZE = 128;
int bvh_stack[BVH_STACK_SIZE];
int bvhStackTop = 0;

/*********************************************************************************/
Triangle[12] createCube(vec3 origin) {
    return Triangle[12](
        // Front face
        Triangle(vec3(-1.0, -1.0, 1.0) + origin, vec3(1.0, -1.0, 1.0) + origin, vec3(1.0, 1.0, 1.0) + origin),
        Triangle(vec3(-1.0, -1.0, 1.0) + origin, vec3(1.0, 1.0, 1.0) + origin, vec3(-1.0, 1.0, 1.0) + origin),

        // Back face
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(-1.0, 1.0, -1.0) + origin, vec3(1.0, 1.0, -1.0) + origin),
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(1.0, 1.0, -1.0) + origin, vec3(1.0, -1.0, -1.0) + origin),

        // Left face
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(-1.0, -1.0, 1.0) + origin, vec3(-1.0, 1.0, 1.0) + origin),
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(-1.0, 1.0, 1.0) + origin, vec3(-1.0, 1.0, -1.0) + origin),

        // Right face
        Triangle(vec3(1.0, -1.0, -1.0) + origin, vec3(1.0, 1.0, 1.0) + origin, vec3(1.0, -1.0, 1.0) + origin),
        Triangle(vec3(1.0, -1.0, -1.0) + origin, vec3(1.0, 1.0, -1.0) + origin, vec3(1.0, 1.0, 1.0) + origin),

        // Top face
        Triangle(vec3(-1.0, 1.0, -1.0) + origin, vec3(-1.0, 1.0, 1.0) + origin, vec3(1.0, 1.0, 1.0) + origin),
        Triangle(vec3(-1.0, 1.0, -1.0) + origin, vec3(1.0, 1.0, 1.0) + origin, vec3(1.0, 1.0, -1.0) + origin),

        // Bottom face
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(1.0, -1.0, 1.0) + origin, vec3(-1.0, -1.0, 1.0) + origin),
        Triangle(vec3(-1.0, -1.0, -1.0) + origin, vec3(1.0, -1.0, -1.0) + origin, vec3(1.0, -1.0, 1.0) + origin)
    );
}

// Push a ray onto the stack
void push(Ray ray) {
    if (stackTop < STACK_SIZE) {
        stack[stackTop] = ray;
        stackTop++;
    }
}

void pushb(int index) {
    if (bvhStackTop < BVH_STACK_SIZE) {
        bvh_stack[bvhStackTop] = index;
        bvhStackTop++;
    }
}

// Pop a ray from the stack
Ray pop() {
    if (stackTop > 0) {
        stackTop--;
        return stack[stackTop];
    }
    // Return a dummy ray if the stack is empty
    return Ray(vec3(0.0), vec3(0.0), -1);
}

int popb() {
    if (bvhStackTop > 0) {
        bvhStackTop--;
        return bvh_stack[bvhStackTop];
    }
    // Return a dummy ray if the stack is empty
    return -1;
}

// Check if the stack is empty
bool isEmpty() {
    return stackTop == 0;
}

bool bvh_isEmpty() {
    return bvhStackTop == 0;
}

vec3 getTriangleCenter(Triangle tri) {
    return (tri.v0 + tri.v1 + tri.v2) / 3.0;
}

bool pointInTriangle(vec3 pt, vec3 v0, vec3 v1, vec3 v2) {
    vec3 v0v1 = v1 - v0;
    vec3 v0v2 = v2 - v0;
    vec3 v0pt = pt - v0;

    float dot00 = dot(v0v2, v0v2);
    float dot01 = dot(v0v2, v0v1);
    float dot02 = dot(v0v2, v0pt);
    float dot11 = dot(v0v1, v0v1);
    float dot12 = dot(v0v1, v0pt);

    float invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

float intersectsTriangle(Triangle v, Ray r) {
    vec3 edge1 = v.v1 - v.v0;
    vec3 edge2 = v.v2 - v.v0; // All points of triangle are on the same plane
    vec3 normal = cross(edge1, edge2); // meaning the normal Doted with any point that may lie on the plain should be 0
    float vn = dot(r.direction, normal);
    // if (vn == 0) return -1.0; // Parallel Ray to plane
    float t = dot((v.v0 - r.origin), normal) / vn;
    // if (t < 0) return -1.0; // Ray points away from plane
    vec3 point = r.origin + t * r.direction;
    if (pointInTriangle(point, v.v0, v.v1, v.v2)) return t;
    return -1.0;
}

float intersectsTriangleAlt(int index, Ray r) {
    int verIndex = index * 3;
    Triangle v = Triangle(trivertex[verIndex], trivertex[verIndex + 1], trivertex[verIndex + 2]);
    vec3 edge1 = v.v1 - v.v0;
    vec3 edge2 = v.v2 - v.v0; // All points of triangle are on the same plane
    vec3 normal = normalize(cross(edge2, edge1)); // meaning the normal Doted with any point that may lie on the plain should be 0
    float vn = dot(r.direction, normal);
    //if (vn == 0.0) return -1.0; // Parallel Ray to plane
    float t = dot((v.v0 - r.origin), normal) / vn;
    if (t < 0.0) return -1.0; // Ray points away from plane
    vec3 point = r.origin + t * r.direction;
    if (pointInTriangle(point, v.v0, v.v1, v.v2)) return t;
    return -1.0;
}

bool isInShadowTriangle(Triangle cube[hittableCount], Ray r, int originObject, float distance) {
    for (int i = 0; i < hittableCount; i++) {
        if (i == originObject) {
            continue;
        }
        float tShadow = intersectsTriangle(cube[i], r);
        if (tShadow > 0.0 && tShadow < distance) {
            return true;
        }
    }

    return false;
}

bool isInShadowTriangle2(Ray r, int originObject, float distance) {
    for (int i = 0; i < hittableCount; i++) {
        if (i == originObject) {
            continue;
        }
        float tShadow = intersectsTriangle(trianglesBuffer[i], r);
        if (tShadow > 0.0 && tShadow < distance) {
            return true;
        }
    }

    return false;
}

bool isInShadowTriangleAlt(Ray r, int originObject, float distance) {
    for (int i = 0; i < hittableCount; i++) {
        if (i == originObject) {
            continue;
        }

        float tShadow = intersectsTriangleAlt(triIdx[i], r);
        if (tShadow > 0.0 && tShadow < distance) {
            return true;
        }
    }

    return false;
}

float intersectsBoxWithDistance(vec3 bboxMin, vec3 bboxMax, Ray ray) {
    vec3 invDir = 1.0 / ray.direction;
    vec3 tMin = (bboxMin - ray.origin) * invDir;
    vec3 tMax = (bboxMax - ray.origin) * invDir;

    vec3 t1 = min(tMin, tMax); // Entry points
    vec3 t2 = max(tMin, tMax); // Exit points

    float tNear = max(max(t1.x, t1.y), t1.z); // Closest intersection
    float tFar = min(min(t2.x, t2.y), t2.z); // Farthest intersection

    // Check if the ray intersects the box
    if (tNear > tFar || tFar < 0.0) {
        return 1e10; // Large value for no intersection
    }

    return tNear > 0.0 ? tNear : tFar; // Return the valid intersection distance
}

bool intersectsBox(vec3 bboxMin, vec3 bboxMax, Ray ray) {
    vec3 invDir = 1.0 / ray.direction;
    vec3 tMin = (bboxMin - ray.origin) * invDir;
    vec3 tMax = (bboxMax - ray.origin) * invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar > 0.0;
}

float intersectBoxDistance(vec3 bboxMin, vec3 bboxMax, Ray ray) {
    vec3 invDir = 1.0 / ray.direction; // Compute the inverse of the ray direction
    vec3 tMin = (bboxMin - ray.origin) * invDir;
    vec3 tMax = (bboxMax - ray.origin) * invDir;

    vec3 t1 = min(tMin, tMax); // Entry points for the ray
    vec3 t2 = max(tMin, tMax); // Exit points for the ray

    float tNear = max(max(t1.x, t1.y), t1.z); // Closest intersection
    float tFar = min(min(t2.x, t2.y), t2.z); // Furthest intersection

    // Check if ray intersects the box
    if (tNear > tFar || tFar < 0.0) {
        return 1e10; // Return a large value if no intersection
    }

    return tNear > 0.0 ? tNear : tFar; // Return the valid intersection distance
}


vec3 getTriangleColor(int i) {
    // Define a color palette for the 12 triangles
    vec3 colors[12];
    colors[0] = vec3(1.0, 0.0, 0.0); // Red
    colors[1] = vec3(0.0, 1.0, 0.0); // Green
    colors[2] = vec3(0.0, 0.0, 1.0); // Blue
    colors[3] = vec3(1.0, 1.0, 0.0); // Yellow
    colors[4] = vec3(1.0, 0.0, 1.0); // Magenta
    colors[5] = vec3(0.0, 1.0, 1.0); // Cyan
    colors[6] = vec3(0.5, 0.5, 0.5); // Gray
    colors[7] = vec3(1.0, 0.5, 0.0); // Orange
    colors[8] = vec3(0.5, 0.0, 1.0); // Purple
    colors[9] = vec3(0.0, 0.5, 0.5); // Teal
    colors[10] = vec3(0.5, 1.0, 0.0); // Lime
    colors[11] = vec3(0.0, 0.0, 0.0); // Black

    // Ensure the index is within bounds
    if (i < 0 || i >= 12) {
        return vec3(1.0, 1.0, 1.0); // Default color (white) for out-of-bounds index
    }

    return colors[i];
}

vec4 proccessRayBVHAlt(Ray r, Light emitter[emitterCount_max]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength
    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();
        if (currentRay.depth >= MAX_RECURSION_DEPTH) continue;
        float t = 5e+10;
        int index = -1;

        pushb(0);
        while (!bvh_isEmpty()) {
            int currentNodeIndex = popb();
            BVHNode currentNode = nodes[currentNodeIndex];
            if (!intersectsBox(currentNode.bboxMin, currentNode.bboxMax, currentRay)) {
                continue;
            }

            if (currentNode.isLeaf == 1) {
                for (int i = 0; i < currentNode.count; ++i) {
                    int firstVertexIndex = triIdx[currentNode.start + i];
                    vec3 v00 = trivertex[firstVertexIndex*3];
                    vec3 v11 = trivertex[firstVertexIndex*3 + 1];
                    vec3 v22 = trivertex[firstVertexIndex*3 + 2];
                    Triangle tri = Triangle(v00, v11, v22);

                    float temp = intersectsTriangle(tri, currentRay);
                    if (temp < t && temp > 0.0) {
                        t = temp;
                        index = currentNode.start + i;
                    }
                }
            } else {
                //Calculate nearest BB
                BVHNode leftChild = nodes[currentNode.leftChild];
                BVHNode rightChild = nodes[currentNode.rightChild];
                float distLeft = intersectsBoxWithDistance(leftChild.bboxMin, leftChild.bboxMax, currentRay);
                float distRight = intersectsBoxWithDistance(rightChild.bboxMin, rightChild.bboxMax, currentRay);

                if (distLeft < distRight) {
                    if (currentNode.rightChild >= 0 && distRight < 1e10) pushb(currentNode.rightChild);
                    if (currentNode.leftChild >= 0 && distLeft < 1e10) pushb(currentNode.leftChild);
                } else {
                    if (currentNode.leftChild >= 0 && distLeft < 1e10) pushb(currentNode.leftChild);
                    if (currentNode.rightChild >= 0 && distRight < 1e10) pushb(currentNode.rightChild);
                }
            }
        }

        if (t >= 0.0 && index != -1) {

            // Intersection point & normal
            vec3 sectionPoint = currentRay.origin + t * currentRay.direction;

            int v = triIdx[index] * 3;
            vec3 edge1 = trivertex[v + 1] - trivertex[v];
            vec3 edge2 = trivertex[v + 2] - trivertex[v];

            vec3 N = normalize(cross(edge1, edge2));
            vec3 localColor = vec3(0.0);
            bool anyLightHit = false;

            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];

                vec3 shadowRay = normalize(light.position - sectionPoint);
                float distanceToLight = length(light.position - sectionPoint);
                float attenuation = 1.0 / (distanceToLight * distanceToLight);

                bool isShadow = isInShadowTriangleAlt(Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);

                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0);
                    vec3 lighting = reflec_accumulation * light.color * materials[matIndex[index]].color * light.intensity * diffuse * attenuation;
                    localColor += lighting;
                    anyLightHit = true;
                }
            }

            color += localColor;
            if (anyLightHit && length(reflec_accumulation) > 0.01) {
                vec3 reflecDirection = reflect(currentRay.direction, N);
                reflec_accumulation *= materials[matIndex[index]].reflection;
                Ray nextRay = Ray(sectionPoint + 0.01 * N, reflecDirection, currentRay.depth + 1);
                push(nextRay);
            }
        }
    }
    return vec4(color, 0.0);
}

vec4 proccessRayBVH(Ray r, Light emitter[emitterCount_max]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength

    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();
        if (currentRay.depth == -1) break;
        // Terminate if the recursion depth is too high
        if (currentRay.depth >= 5) continue;
        float t = 5e+10;
        uint index = 100000;

        pushb(0); // Push Root node;
        while (!bvh_isEmpty()) {
            int currentNodeIndex = popb();
            if (currentNodeIndex == -1) continue;
            BVHNode currentNode = nodes[currentNodeIndex];
            if (!intersectsBox(currentNode.bboxMin, currentNode.bboxMax, currentRay)) {
                continue;
            }

            if (currentNode.isLeaf == 1) {
                for (int i = 0; i < currentNode.count; i++) {
                    uint primitivIndex = triIdx[currentNode.start + i];
                    float temp = intersectsTriangleAlt(triIdx[currentNode.count + i], currentRay);
                    if (temp < t && temp > 0.0) {
                        t = temp;
                        index = primitivIndex;
                    }
                }
            } else {
                if (currentNode.rightChild >= 0) pushb(currentNode.rightChild);
                if (currentNode.leftChild >= 0) pushb(currentNode.leftChild);
            }
        }

        if (t > 0.0 && index == 100000) {
            // Intersection point & normal
            vec3 sectionPoint = currentRay.origin + t * currentRay.direction;

            uint v = triIdx[index] * 3;
            vec3 edge1 = trivertex[v + 1] - trivertex[v];
            vec3 edge2 = trivertex[v + 2] - trivertex[v];

            // vec3 edge1 = trianglesBuffer[index].v1 - trianglesBuffer[index].v0;
            // vec3 edge2 = trianglesBuffer[index].v2 - trianglesBuffer[index].v0;
            vec3 N = normalize(cross(edge1, edge2));
            vec3 localColor = vec3(0.0);
            bool anyLightHit = false;

            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];

                vec3 shadowRay = normalize(light.position - sectionPoint);
                float distanceToLight = length(light.position - sectionPoint);
                float attenuation = 1.0 / (distanceToLight * distanceToLight);

                // bool isShadow = isInShadowTriangleAlt(Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);
                bool isShadow = false;

                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0);
                    vec3 lighting = reflec_accumulation * light.color * vec3(0.4, 0.0, 0.2) * light.intensity * diffuse * attenuation;
                    localColor += lighting;
                    anyLightHit = true;
                }
            }

            color += localColor;
            if (anyLightHit && length(reflec_accumulation) > 0.01) {
                vec3 reflecDirection = reflect(currentRay.direction, N);
                reflec_accumulation *= 0.8;
                Ray nextRay = Ray(sectionPoint + 0.01 * N, reflecDirection, currentRay.depth + 1);
                push(nextRay);
            }
        }
    }
    return vec4(color, 1.0);
}

vec4 proccessRay1(Triangle cube[hittableCount], Ray r, Light emitter[emitterCount_max]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength

    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();
        if (currentRay.depth == -1) break;
        // Terminate if the recursion depth is too high
        if (currentRay.depth >= 5) continue;

        float t = 5e+10;
        int index = -1;

        // Find the closest intersection
        for (int i = 0; i < hittableCount; i++) {
            float temp = intersectsTriangle(cube[i], currentRay);
            if (temp < t && temp > 0.0) {
                t = temp;
                index = i;
            }
        }

        if (t > 0.0 && index >= 0) {
            // Intersection point & normal
            vec3 sectionPoint = currentRay.origin + t * currentRay.direction;
            vec3 edge1 = cube[index].v1 - cube[index].v0;
            vec3 edge2 = cube[index].v2 - cube[index].v0;
            vec3 N = normalize(cross(edge1, edge2));

            // Accumulate light contribution from all light sources first
            vec3 localColor = vec3(0.0);
            bool anyLightHit = false;

            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];

                // Shadow ray setup
                vec3 shadowRay = normalize(light.position - sectionPoint);
                float distanceToLight = length(light.position - sectionPoint);
                float attenuation = 1.0 / (distanceToLight * distanceToLight);

                bool isShadow = isInShadowTriangle(cube, Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);

                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0);
                    vec3 lighting = reflec_accumulation * light.color * vec3(0.4, 0.0, 0.2) * light.intensity * diffuse * attenuation;
                    localColor += lighting;
                    anyLightHit = true;
                }
            }

            color += localColor;

            if (anyLightHit && length(reflec_accumulation) > 0.01) {
                vec3 reflecDirection = reflect(currentRay.direction, N);
                reflec_accumulation *= 0.8; // Reduce reflection strength
                Ray nextRay = Ray(sectionPoint + 0.01 * N, reflecDirection, currentRay.depth + 1);
                push(nextRay);
            }
        }
    }

    return vec4(color, 1.0);
}

vec4 proccessRaySSBO(Ray r, Light emitter[emitterCount_max]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength

    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();
        if (currentRay.depth == -1) break;
        // Terminate if the recursion depth is too high
        if (currentRay.depth >= 5) continue;

        float t = 5e+10;
        int index = -1;

        // Find the closest intersection
        for (int i = 0; i < hittableCount; i++) {
            float temp = intersectsTriangle(trianglesBuffer[i], currentRay);
            if (temp < t && temp > 0.0) {
                t = temp;
                index = i;
            }
        }

        if (t > 0.0 && index >= 0) {
            // Intersection point & normal
            vec3 sectionPoint = currentRay.origin + t * currentRay.direction;
            vec3 edge1 = trianglesBuffer[index].v1 - trianglesBuffer[index].v0;
            vec3 edge2 = trianglesBuffer[index].v2 - trianglesBuffer[index].v0;
            vec3 N = normalize(cross(edge1, edge2));

            // Accumulate light contribution from all light sources first
            vec3 localColor = vec3(0.0);
            bool anyLightHit = false;

            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];

                // Shadow ray setup
                vec3 shadowRay = normalize(light.position - sectionPoint);
                float distanceToLight = length(light.position - sectionPoint);
                float attenuation = 1.0 / (distanceToLight * distanceToLight);

                bool isShadow = isInShadowTriangle2(Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);

                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0);
                    vec3 lighting = reflec_accumulation * light.color * vec3(0.4, 0.0, 0.2) * light.intensity * diffuse * attenuation;
                    localColor += lighting;
                    anyLightHit = true;
                }
            }

            color += localColor;

            if (anyLightHit && length(reflec_accumulation) > 0.01) {
                vec3 reflecDirection = reflect(currentRay.direction, N);
                reflec_accumulation *= 0.8; // Reduce reflection strength
                Ray nextRay = Ray(sectionPoint + 0.01 * N, reflecDirection, currentRay.depth + 1);
                push(nextRay);
            }
        }
    }

    return vec4(color, 1.0);
}

vec4 CalcColorWithLightSourcesTriangle(Triangle cube[hittableCount], Ray r, Light emitter[emitterCount_max]) {
    int MAXIMAL_BOUNCES = 3;
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); //Reflection in percent 100% red 100% green and blue

    //For each bounce we want:
    for (int bounce = 0; bounce <= MAXIMAL_BOUNCES; bounce++) {
        float t = 1.0 / 0.0; // =)
        int index = -1;
        //To determine the first object we hit from camera to pixel;
        for (int i = 0; i < hittableCount; i++) {
            float temp = intersectsTriangle(cube[i], r);
            if (temp < t && temp > 0.0) {
                t = temp;
                index = i;
            }
        }

        //t = position of intersection if it exists.
        if (t > 0.0 && index >= 0) {
            // We calculate the intersection point;
            vec3 sectionPoint = r.origin + t * r.direction; // intersection point on the object
            // We calculate the normal of the intersection point;
            //vec3 N = normalize(sectionPoint - getTriangleCenter(cube.triangles[index]));
            vec3 edge1 = cube[index].v1 - cube[index].v0;
            vec3 edge2 = cube[index].v2 - cube[index].v0;
            vec3 N = normalize(cross(edge1, edge2));

            // For each lightsource we want;
            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];
                // To calculate the shadow ray to the light source;
                vec3 shadowRay = normalize(light.position - sectionPoint);
                //the distance to the light to the section point;
                float distanceToLight = length(light.position - sectionPoint);
                //The attentuation based on Intensity = 1 / d^2
                float attenuation = 1.0 / (distanceToLight * distanceToLight); // Inverse square law
                // Check if the shadow ray intersects other objects in its path
                bool isShadow = isInShadowTriangle(cube, Ray(sectionPoint + 0.01, shadowRay, 0), index, distanceToLight);
                // If no intersections, apply direct lighting (diffuse lighting)
                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0); // Lambertian diffuse shading
                    vec3 lighting = reflec_accumulation * light.color * vec3(0.4, 0.0, 0.2) * light.intensity * diffuse * attenuation;
                    color += lighting;

                    vec3 reflecDirection = reflect(r.direction, N);
                    //sectionPoint = sectionPoint * reflecDirection;
                    r = Ray(sectionPoint + 0.01, reflecDirection, 0);
                    reflec_accumulation *= 0.8;
                    if (length(reflec_accumulation) < 0.01) break; // If accumulation reach a certain point there should be no further reflections and the bounces should stop
                }
            }
        }
    }
    return vec4(color, 1.0);
}

vec4 rayColor(Ray r) {
    Triangle[12] trianglesCube1 = createCube(vec3(2.0, 0.0, 0.0));
    Triangle[12] trianglesCube2 = createCube(vec3(-2.0, 0.0, 0.0));
    Triangle[12] trianglesCube3 = createCube(vec3(0.0, -2.0, 0.0));
    Triangle[36] merged;

    // Copy triangles from the first cube
    for (int i = 0; i < 12; i++) {
        merged[i] = trianglesCube1[i];
    }

    // Copy triangles from the second cube
    for (int i = 0; i < 12; i++) {
        merged[i + 12] = trianglesCube2[i];
    }

    for (int i = 0; i < 12; i++) {
        merged[i + 24] = trianglesCube3[i];
    }

    // Light[1] lightSources;
    // vec3 position = vec3(0.0, 0.0 + 3 * tan(time), 5 + 3 * sin(time));
    // lightSources[0] = Light(position, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), 10.0);	

    Light[emitterCount_max] lightSources;
    for(int i = 0; i < ls_active_light_sources; i++){
      lightSources[i] = Light(ls_positions[i], ls_directions[i], ls_colors[i], ls_intensities[i]);
	}

    //return proccessRaySSBO(r, lightSources);
    return proccessRayBVHAlt(r, lightSources);
    //return CalcColorWithLightSourcesTriangle(trianglesCube1, r, lightSources);
}

void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 dims = imageSize(textureOutput);
    float x = float(pixelCoords.x * 2 - dims.x) / dims.x; // transforms to [-1.0, 1.0]
    float y = float(pixelCoords.y * 2 - dims.y) / dims.y; // transforms to [-1.0, 1.0]

    vec3 rayOrigin = cameraPos - vec3(0.0, 5.0, 5.0); // Origin of the inital ray

    //ray in clip
    vec4 rayPixel = vec4(x, y, -1.0, 1.0);

    // Convert to eye space
    vec4 rayEye = inverse(Projection) * rayPixel;
    rayEye = vec4(rayEye.xy, -1.0, 0.0); // We = 0 dir

    // Convert to world space
    vec3 rayDirWorld = normalize(vec3(inverse(View) * rayEye));

    //vec3 rayDirection = normalize(rayPixel.xyz - rayOrigin);
    Ray r = Ray(rayOrigin, rayDirWorld, 0);
    //imageStore(textureOutput, pixelCoords, vec4(rayDirWorld, 1.0));
    imageStore(textureOutput, pixelCoords, rayColor(r));
}
