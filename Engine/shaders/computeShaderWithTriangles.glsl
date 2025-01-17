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
//IMPORTANT
uniform int bounce;
uniform int hittable;

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

// struct Material {
//     vec3 color;	// Diffuse Color
// 	float pad0;
// 	vec3 Ka;	// Ambient Color
// 	float pad1;
// 	vec3 Ks;	// Specular Color
// 	float pad2;
// 	float d;	// Dissolve
// 	int illum;	// Illumination
//     float reflection;	// Specular Exponent
// 	float Ni;	// optical density
// };

struct Material {
    vec3 color; // Diffuse Color
    float reflection;
    vec3 Ka; // Ambient Color
    float Ni; // optical density
    vec3 Ks; // Specular Color
    float d; // Dissolve
    int illum; // Illumination
    float pad0;
    float pad1;
    float pad2;
};

layout(std430, binding = 5) buffer MaterialBuffer {
    Material materials[];
};

struct Vec3Padded {
    vec3 data;
    float pad;
    vec3 normal;
    float pad0;
};

layout(std430, binding = 4) buffer VertexBuffer {
    Vec3Padded trivertex[];
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
int hittableCount = 60;
int MAX_RECURSION_DEPTH = 4;

const int STACK_SIZE = 128;
Ray stack[STACK_SIZE];
int stackTop = 0;

const int BVH_STACK_SIZE = 128;
int bvh_stack[BVH_STACK_SIZE];
int bvhStackTop = 0;
vec3 ambientLightColor = vec3(0.0);
/*********************************************************************************/
// STACK OPERATIONS

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

/*********************************************************************************/
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
    Triangle v = Triangle(trivertex[verIndex].data, trivertex[verIndex + 1].data, trivertex[verIndex + 2].data);
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

//https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
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
struct bvh_return {
    float t;
    int index;
};

bvh_return processBVH(Ray currentRay) {
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
                vec3 v00 = trivertex[firstVertexIndex * 3].data;
                vec3 v11 = trivertex[firstVertexIndex * 3 + 1].data;
                vec3 v22 = trivertex[firstVertexIndex * 3 + 2].data;
                Triangle tri = Triangle(v00, v11, v22);

                float temp = intersectsTriangle(tri, currentRay);
                if (temp < t && temp > 0.0) {
                    t = temp;
                    index = currentNode.start + i;
                }
            }
        } else {
            //Calculate nearest BB
            pushb(currentNode.leftChild);
            pushb(currentNode.rightChild);
        }
    }
    return bvh_return(t, index);
}

bool processBVH_Shadow(Ray currentRay, int originObject, float distance) {
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
                if (currentNode.start + i == originObject) continue;
                int firstVertexIndex = triIdx[currentNode.start + i];
                vec3 v00 = trivertex[firstVertexIndex * 3].data;
                vec3 v11 = trivertex[firstVertexIndex * 3 + 1].data;
                vec3 v22 = trivertex[firstVertexIndex * 3 + 2].data;
                Triangle tri = Triangle(v00, v11, v22);

                float temp = intersectsTriangle(tri, currentRay);
                if (temp < t && temp > 0.0) {
                    t = temp;
                    index = currentNode.start + i;
                }
            }
        } else {
            //Calculate nearest BB
            pushb(currentNode.leftChild);
            pushb(currentNode.rightChild);
        }
    }

    if (t > 0.0 && t < distance) return true;
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

//https://www.cs.cornell.edu/courses/cs4620/2019fa/slides/07.5rt-interp.pdf :: Folie 7
//https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
vec3 bayecentricCalculation(vec3 intersectionPoint, vec3 v0, vec3 v1, vec3 v2) {
    // Calculate the edges of the triangle
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    // Vector from v0 to the intersection point
    vec3 vp = intersectionPoint - v0;

    // Precompute dot products for the triangle
    float d00 = dot(edge1, edge1);
    float d01 = dot(edge1, edge2);
    float d11 = dot(edge2, edge2);
    float d20 = dot(vp, edge1);
    float d21 = dot(vp, edge2);

    // Compute the denominator of the barycentric coordinates
    float denom = d00 * d11 - d01 * d01;

    // Calculate barycentric coordinates
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0 - v - w;

    // Return the barycentric coordinates
    return vec3(u, v, w);
}

vec3 backgroundColor(vec3 direction) {
    float t = 0.5 * (normalize(direction).y + 1.0);
    return mix(vec3(0.1, 0.1, 0.2), vec3(0.5, 0.7, 1.0), t); // Dark blue to light blue
}
// Actual Raytrace Function
vec4 proccessRayBVHAlt(Ray r, Light emitter[emitterCount_max]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength
    bool hit = false;
    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();
        if (currentRay.depth >= MAX_RECURSION_DEPTH) continue;
        //bvh_return ret = bvh_return(5e+10,-1);
        bvh_return ret = processBVH(currentRay);
        float t = ret.t;
        int index = ret.index;

        if (t >= 0.0 && index != -1) {

            // Intersection point & normal
            vec3 sectionPoint = currentRay.origin + t * currentRay.direction;

            int v = triIdx[index] * 3;
            vec3 v0 = trivertex[v].data;
            vec3 v1 = trivertex[v + 1].data;
            vec3 v2 = trivertex[v + 2].data;
            //vec3 edge1 = trivertex[v + 1].data - trivertex[v].data;
            //vec3 edge2 = trivertex[v + 2].data - trivertex[v].data;
            vec3 baryCoords = bayecentricCalculation(sectionPoint, v0, v1, v2);
            //vec3 N = normalize(cross(edge1, edge2));
            vec3 N = normalize(
                    baryCoords.x * trivertex[v].normal +
                        baryCoords.y * trivertex[v + 1].normal +
                        baryCoords.z * trivertex[v + 2].normal
                );
            vec3 localColor = vec3(0.0);
            bool anyLightHit = false;

            // Ambient Lighting
            vec3 ambient = materials[matIndex[index]].Ka * ambientLightColor;
            color += ambient;
            for (int lIndex = 0; lIndex < ls_active_light_sources; lIndex++) {
                Light light = emitter[lIndex];

                vec3 shadowRay = normalize(light.position - sectionPoint);
                float distanceToLight = length(light.position - sectionPoint);
                float attenuation = 1.0 / (distanceToLight * distanceToLight);

                //bool isShadow = isInShadowTriangleAlt(Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);
                bool isShadow = processBVH_Shadow(Ray(sectionPoint + 0.01 * N, shadowRay, currentRay.depth), index, distanceToLight);
                //bool isShadow = false;
                if (!isShadow) {
                    float diffuse = max(dot(N, shadowRay), 0.0);
                    vec3 lighting = reflec_accumulation * light.color * materials[matIndex[index]].color * light.intensity * diffuse * attenuation;

                    localColor += lighting;
                    anyLightHit = true;
                }
            }

            color += localColor;
            if (anyLightHit && length(reflec_accumulation) > 0.001) {
                vec3 reflecDirection = reflect(currentRay.direction, N);
                reflec_accumulation *= materials[matIndex[index]].reflection;
                Ray nextRay = Ray(sectionPoint + 0.01 * N, reflecDirection, currentRay.depth + 1);
                push(nextRay);
            }

            if (materials[matIndex[index]].illum == 4) {
                vec3 refractDirection = refract(currentRay.direction, N, materials[matIndex[index]].Ni);
                Ray refractRay = Ray(sectionPoint - 0.01 * N, refractDirection, currentRay.depth + 1);
                push(refractRay);
            }
            color *= materials[matIndex[index]].d;
            hit = true;
        } else {
            color += reflec_accumulation * backgroundColor(currentRay.direction);
        }
    }
    //if(!hit){color = vec3(1.0f,0.0f,0.0f);}
    return vec4(color, 0.0);
}

vec4 rayColor(Ray r) {

    // Light[1] lightSources;
    // vec3 position = vec3(0.0, 0.0 + 3 * tan(time), 5 + 3 * sin(time));
    // lightSources[0] = Light(position, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), 10.0);

    Light[emitterCount_max] lightSources;
    for (int i = 0; i < ls_active_light_sources; i++) {
        lightSources[i] = Light(ls_positions[i], ls_directions[i], ls_colors[i], ls_intensities[i]);
    }

    //return proccessRaySSBO(r, lightSources);
    return proccessRayBVHAlt(r, lightSources);
    //return CalcColorWithLightSourcesTriangle(trianglesCube1, r, lightSources);
}

void handleUniforms() {
    hittableCount = hittable;
    MAX_RECURSION_DEPTH = bounce;
}

void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 dims = imageSize(textureOutput);
    float x = float(pixelCoords.x * 2 - dims.x) / dims.x; // transforms to [-1.0, 1.0]
    float y = float(pixelCoords.y * 2 - dims.y) / dims.y; // transforms to [-1.0, 1.0]
    handleUniforms();
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
