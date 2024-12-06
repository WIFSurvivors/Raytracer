#version 430 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

// Model does not really exist rn so bleh
uniform float time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 cameraPos;

/*********************************************************************************/
//STRUCTS

struct Triangle {
    vec3 v0;
    vec3 v1;
    vec3 v2;
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
const int hittableCount = 36;
const int emitterCount = 1;
const int MAX_RECURSION_DEPTH = 3;

const int STACK_SIZE = 128;
Ray stack[STACK_SIZE];
int stackTop = 0;

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

// Pop a ray from the stack
Ray pop() {
    if (stackTop > 0) {
        stackTop--;
        return stack[stackTop];
    }
    // Return a dummy ray if the stack is empty
    return Ray(vec3(0.0), vec3(0.0), -1);
}

// Check if the stack is empty
bool isEmpty() {
    return stackTop == 0;
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

vec4 proccessRay(Triangle cube[hittableCount], Ray r, Light emitter[emitterCount]) {
    vec3 color = vec3(0.0);
    vec3 reflec_accumulation = vec3(1.0); // Reflection strength

    push(r);
    while (!isEmpty()) {
        Ray currentRay = pop();

        // Terminate if the recursion depth is too high
        if (currentRay.depth >= 5) break;

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

            for (int lIndex = 0; lIndex < emitterCount; lIndex++) {
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

vec4 CalcColorWithLightSourcesTriangle(Triangle cube[hittableCount], Ray r, Light emitter[emitterCount]) {
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
            for (int lIndex = 0; lIndex < emitterCount; lIndex++) {
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

    Light[1] lightSources;
    vec3 position = vec3(0.0, 10.0, 3.0) + 3 * sin(time);
    lightSources[0] = Light(position, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), 100.0);
    return CalcColorWithLightSourcesTriangle(merged, r, lightSources);
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
