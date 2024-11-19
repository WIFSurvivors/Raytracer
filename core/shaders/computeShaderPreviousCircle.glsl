#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

// Model does not really exist rn so bleh
uniform float time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

struct Sphere {
    float r;
    vec3 C;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

// ax+by+cz-d=0
struct Plane {
    vec3 normal;
    float distance;
    vec3 position;
};

struct Vertex {
    vec3 v0;
    vec3 v1;
    vec3 v2;
};

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

float intersectsTriangle(Vertex v, Ray r) {
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

float intersectsSphere(Sphere s, Ray r) {
    vec3 oc = s.C - r.origin;
    float a = length(r.direction) * length(r.direction);
    float h = dot(r.direction, oc);
    float c = length(oc) * length(oc) - s.r * s.r;

    float D = h * h - (a * c);

    if (D <= 0) return -1.0;
    return (h - sqrt(D)) / a;
}

float rand(vec2 co) {
    // vec2 co = veh2(0.7281, 13.23);
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 randOnHemisphere(vec3 n) {
    vec3 r = vec3(rand(n.xy), rand(n.yz), rand(n.xz));
    if (dot(r, n) > 0) {
        return r;
    } else {
        return -r;
    }
}

vec4 rayColor(Ray r) {
    mat4 MVP = Projection * View * Model * mat4(1.0);
    vec3 v0 = vec3(-0.6, -0.8, -0.5);
    vec3 v1 = vec3(0.6, -0.8, -0.5);
    vec3 v2 = vec3(0.0, 0.8, 0.2);

    Vertex v = Vertex(v0, v1, v2);

    vec4 c = vec4(0.0, 0.0, -1.0, 1.0);
    Sphere s1 = Sphere(0.9, c.xyz);

    // Sphere s2 = Sphere(0.5, vec3(0.0, 0.4, 0.0));

    float t = -1.0;
    float temp = -1.0;

    t = intersectsSphere(s1, r);
    // if (temp != -1.0) {
    //     if (t == -1) {
    //         t = temp;
    //     } else {
    //         t = min(t, temp);
    //     }
    // }

    // t = min(max(-1, t), max(-1 , intersectsSphere(s1, r)));
    // t = min(max(-1, t), max(-1 , intersectsTriangle(v, r)));

    // temp = intersectsSphere(s2, r);
    // if (temp != -1.0) {
    // if (t == -1) {
    // t = temp;
    // } else {
    // t = min(t, temp);
    // }
    // }

    /*
                	temp = intersectsSphere(s2, r);
                	if(temp != -1.0) {
                		if(t == -1) {
                			t = temp;
                		} else {
                			t = min(t, temp);
                		}
                	}
                */
    if (t > 0.0) {
        vec3 N = normalize(r.origin + t * r.direction - s1.C);
        vec3 dir = randOnHemisphere(N);
        return vec4(dir, 1.0);
    }

    // if(t > 0.0) {
    //	vec3 pixelCol = vec3(1.0, 1.0, 1.0);
    //	vec3 p = r.origin + t * r.direction;
    //	for(int i = 0; i < 70; i++) {
    //		vec3 N = normalize(p - vec3(0.0, 0.0, -1.0));
    // return 0.5 * vec4(N.x + 1.0, N.y + 1.0, N.z + 1.0, 1.0);
    // vec3 dir = randOnHemisphere(N);

    // pixelCol = pixelCol - 0.02 * dir;
    // p = N + t*dir;
    // }
    // return vec4(pixelCol, 1.0);
    // vec3 N = normalize((r.origin + t * r.direction) - vec3(0.0, 0.0, -1.0));
    // return 0.5 * vec4(N.x + 1.0, N.y + 1.0, N.z + 1.0, 1.0);
    // vec3 dir01 = randOnHemisphere(N);
    // vec3 dir02 = randOnHemisphere(N);
    // vec3 dir03 = randOnHemisphere(N);
    // return 0.8 * vec4(dir01.xyz*0.3 + dir03.xyz*0.3 + dir03.xyz*0.3, 1.0);
    // }
    vec3 N = normalize(r.direction);
    float a = 0.5 * (N.y + 1.0);
    vec3 res = (1.0 - a) * vec3(0.0, 0.0, 0.0) + a * vec3(0.5, 0.7, 1.0);
    return vec4(res, 1.0);
}

Plane createPlane(vec3 point1, vec3 point2, vec3 point3) {
    vec3 p2p1 = point2 - point1;
    vec3 p2p3 = point2 - point3;
    vec3 normal = normalize(cross(p2p1, p2p3));
    float distance = dot(point1, normal);

    return Plane(normal, distance, point1);
}

bool intersectsPlane(Plane p, Ray r) {
    //Plane equation is ax+by+cz=d
    //P(t) = Origin + t * direction
    //a * P(t)x + b * P(t)y + c * P(t)z = d
    //a* (Origin.x + t * direction.x) + ...
    //a * Origin.x + a * t * direction.x + b * Origin.y + b * t * direction.y + c * Origin.z + c * t * direction.z = d
    //a * Origin.x + b * Origin.y + c * Origin.z + t(a * direction.x + b * direction*y + c * direction*z) = d
    //t = d - (a * Origin.x + b * Origin.y + c * Origin.z) / (a * direction.x + b * direction*y + c * direction*z)
    // Denominotr = 0 => Paralel
    // t >= 0 lies before us so intersection comming.

    float denominator = dot(p.normal, r.direction);
    if (denominator == 0.0) return false;
    float t = (p.distance - dot(p.normal, r.origin)) / denominator;
    if (t >= 0) return true;
    return false;
}

void main() {
    ivec2 pixelCoords = ivec2(gl_WorkGroupID.xy);
    ivec2 dims = imageSize(textureOutput);
    float x = float(pixelCoords.x * 2 - dims.x) / dims.x; // transforms to [-1.0, 1.0]
    float y = float(pixelCoords.y * 2 - dims.y) / dims.y; // transforms to [-1.0, 1.0]

    vec3 rayOrigin = vec3(0.0, 0.0, 2.0);
    vec3 rayPixel = vec3(x, y, 0.0);
    vec3 rayDirection = normalize(rayPixel - rayOrigin);
    Ray r = Ray(rayOrigin, rayDirection);
    imageStore(textureOutput, pixelCoords, rayColor(r));
    //imageStore(textureOutput, pixel_coords, vec4(0.0, 1.0, 0.0, 1.0));
}
