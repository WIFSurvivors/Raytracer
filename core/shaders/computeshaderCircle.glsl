#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;



const vec4 triangleColor = vec4(1.0, 0.0, 0.0, 1.0); // Red

struct Sphere{
	float r;
	vec3 C;
};

struct Ray{
	vec3 origin;
	vec3 direction;
};

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

float intersecCircle(Sphere s, Ray r) {
	vec3 oc = s.C - r.origin;
	//float a = length(r.direction) *  length(r.direction);
  float a = dot(r.direction,r.direction);
  float b = -2.0 * dot(r.direction, oc);
	//float h = dot(r.direction, oc);
	//float c = length(oc) * length(oc);
  float c = dot(oc,oc) - s.r * s.r;

	//float D = h * h - (a * c);
  float D = b*b - 4*a*c;
	//if(D < 0) return -1; 
  //return (h - sqrt(D)) / a;

  if (D<=0) return -1.0;
  return (-b - sqrt(D)/(2.0 * a ));
}

vec4 ray_color(Ray r, float t){
  if (t > 0.0) {
    vec3 N = normalize((r.origin + t * r.direction) - vec3(0.0,0.0,-1.0));
    return 0.5 * vec4(N.x +1.0, N.y+1.0,N.z+1.0,1.0);
  }

  vec3 N = normalize(r.direction);
  float a = 0.5*(N.y+1.0);
  vec3 res = (1.0-a)*vec3(1.0,1.0,1.0) + a*vec3(0.5,0.7,1.0);
  return vec4(res,1.0);
}


void main()
{
	ivec2 pixel_coords = ivec2(gl_WorkGroupID.xy);
	
	ivec2 dims = imageSize(textureOutput);
	float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x); // transforms to [-1.0, 1.0]
	float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y); // transforms to [-1.0, 1.0]
	//Sphere spheres[12];
  vec3 rayOrigin = vec3(0.0, 0.0, -2);
  vec3 rayPixel = vec3(x, y, 0.0);
	vec3 rayDirection = normalize(rayPixel - rayOrigin);
	Ray r = Ray(rayOrigin, rayDirection);
	Sphere s = Sphere(0.3, vec3(0.0, 0.0, -1.0));
	float t = intersecCircle(s, r);
  if (t != -1) {
      imageStore(textureOutput, pixel_coords, ray_color(r, t));
      
  }else{
    imageStore(textureOutput, pixel_coords, vec4(0.0, 1.0, 0.0, 1.0));
  }

   
}
