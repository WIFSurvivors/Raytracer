#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

uniform float time;
uniform vec3 v0;
uniform vec3 v1;
uniform vec3 v2;
uniform vec2 imageSize;

bool InTriangle(vec3 A, vec3 B, vec3 C, vec3 P) {
    float w1 = (A.x * (C.y - A.y) + (P.y - A.y) * (C.x - A.x) - P.x * (C.y - A.y)) / ((B.y - A.y) * (C.x - A.x) - (B.x - A.x) * (C.y - A.y));
    float w2 = (P.y - A.y - w1 * (B.y - A.y)) / (C.y - A.y);

    return (w1 >= 0 && w2 >= 0);
}

bool rayIntersectsTriangle(vec3 rayOrigin, vec3 rayDirection, vec3 v0, vec3 v1, vec3 v2) {
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0; // All points of triangle are on the same plane
    vec3 normal = cross(edge1, edge2); // meaning the normal Doted with any point that may lie on the plain should be 0
    float vn = dot(rayDirection, normal);
    if (vn == 0) return false; // Parallel Ray to plane
    float t = dot((v0 - rayOrigin), normal) / vn; // calculates the scale of the ray, so that the ray end will fall on the triangle plane
    if (t < 0) return false; // Ray points away from plane
    vec3 point = rayOrigin + t * rayDirection;
    return InTriangle(v0,v1,v2,point);
}

void main() {
    ivec2 pixelCoord = ivec2(gl_WorkGroupID.xy);
    vec3 rayOrigin = vec3(0.0, 0.0, 1.0);
    //vec3 rayDirection = normalize(vec3(pixelCoord, 0.0) - rayOrigin); //direction vector from origin to pixel
    vec3 rayDirection = normalize(vec3(pixelCoord / imageSize * 2.0 - 1.0, -1.0) - rayOrigin);
	// vec3 rayDirection = normalize(vec3(0.0, 0.0, -0.3));
    vec3 hitColor = vec3(0.0, 1.0, 0.0); // Default color (green)
    // Check for intersection with the triangle
    if (rayIntersectsTriangle(rayOrigin, rayDirection, v0, v1, v2)) {
        hitColor = vec3(1.0, 0.0, 0.0); // Red color for intersection
    }

    // Output the color to the texture
    imageStore(textureOutput, pixelCoord, vec4(hitColor, 1.0));
}
