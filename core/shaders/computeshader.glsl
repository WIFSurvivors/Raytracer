#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

uniform float time;
uniform vec3 v0;
uniform vec3 v1;
uniform vec3 v2;
uniform vec2 imageSize;


const vec4 triangleColor = vec4(1.0, 0.0, 0.0, 1.0); // Red

// Barycentric coordinate function
bool pointInTriangle(vec2 pt, vec2 v0, vec2 v1, vec2 v2) {
    vec2 v0v1 = v1 - v0;
    vec2 v0v2 = v2 - v0;
    vec2 v0pt = pt - v0;

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

bool rayIntersectsTriangle(vec3 rayOrigin, vec3 rayDirection, vec3 V0, vec3 V1, vec3 V2) {
    vec3 edge1 = V1 - V0;
    vec3 edge2 = V2 - V0; // All points of triangle are on the same plane
    vec3 normal = cross(edge1, edge2); // meaning the normal Doted with any point that may lie on the plain should be 0
    float vn = dot(rayDirection, normal);
    if (vn == 0) return false; // Parallel Ray to plane
    float t = dot((V0 - rayOrigin), normal) / vn;
    if (t < 0) return false; // Ray points away from plane
    vec3 point = rayOrigin + t * rayDirection;
    return pointInTriangle(point.xy, V0.xy, V1.xy, V2.xy);
}
vec3 mapToNDC(vec3 objectPosition) {
    float ndcX = (objectPosition.x + 1.0) * 0.5;
    float ndcY = (objectPosition.y + 1.0) * 0.5;
    return vec3(ndcX, ndcY, 0.0);
}
void main()
{
    ivec2 pixelCoord = ivec2(gl_WorkGroupID.xy);
    ivec2 texSize = ivec2(imageSize.xy);
    vec2 uv = vec2(pixelCoord) / vec2(texSize);
    vec3 rayOrigin = vec3(0.0, 0.0, 1.0);
    vec3 rayDirection = vec3(uv, -1.0);
    vec3 hitColor = vec3(0.0, 1.0, 0.0); // Default color (green)

    if (rayIntersectsTriangle(rayOrigin, rayDirection, mapToNDC(v0), mapToNDC(v1), mapToNDC(v2))) {
        imageStore(textureOutput, pixelCoord, triangleColor);
        
    }else{
      imageStore(textureOutput, pixelCoord, vec4(0.0,1.0,0.0,1.0));
    }
}
