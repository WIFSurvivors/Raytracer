#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

// Model does not really exist rn so bleh
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


struct Sphere{
	float r;
	vec3 C;
};

struct Ray{
	vec3 origin;
	vec3 direction;
};

// ax+by+cz-d=0
struct Plane{
  vec3 normal;
  float distance;
  vec3 position;
};

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


Plane createPlane(vec3 point1, vec3 point2, vec3 point3){

  vec3 p2p1 = point2 - point1;
  vec3 p2p3 = point2 - point3;
  vec3 normal = normalize(cross(p2p1,p2p3));
  float distance = dot(point1  ,normal);

  return Plane(normal,distance, point1);

}

bool intersectsPlane(Plane p, Ray r){
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
  if(denominator == 0.0) return false;
  float t = (p.distance - dot(p.normal, r.origin)) / denominator;
  if(t>=0) return true;
  return false;
}



void main(){
  // frick it we just have one vertex for a circle
  //Because we do not translate do not scale
  mat4 MVP = Projection * View * Model * mat4(1.0);
  
  vec3 point1 = vec3(-0.5,0,-0.5);
  vec3 point2 = vec3(-0.5,0,0.5);
  vec3 point3 = vec3(0.5,0.0,0.5);
  
  vec4 clipSpacePoint1 = MVP * vec4(point1,1.0);
  vec4 clipSpacePoint2 = MVP * vec4(point2,1.0);
  vec4 clipSpacePoint3 = MVP * vec4(point3,1.0);

  vec3 circleOrigin = vec3(0.0,5.0,0.0);
  vec4 clipSpacePoint = MVP * vec4(circleOrigin,1.0);
  // To transform from clip to ndc we divide by w w is 1 because point so do nothing
  ivec2 pixel_coords = ivec2(gl_WorkGroupID.xy);

	ivec2 dims = imageSize(textureOutput);
	float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x); // transforms to [-1.0, 1.0]
	float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y); // transforms to [-1.0, 1.0]
  vec3 rayOrigin = vec3(0.0, 0.0, -2.0);
  vec3 rayPixel = vec3(x, y, 0.0);
	vec3 rayDirection = normalize(rayPixel - rayOrigin);
  Sphere s = Sphere(1, vec3(clipSpacePoint.x,clipSpacePoint.y,clipSpacePoint.z));
	Ray r = Ray(rayOrigin, rayDirection);
  
  //draw first plane 
  Plane p = createPlane(clipSpacePoint1.xyz,clipSpacePoint2.xyz,clipSpacePoint3.xyz);
  if (intersectsPlane(p,r)){
    imageStore(textureOutput, pixel_coords, vec4(1.0, 0.0,0.0, 1.0));
  }else {
    
    imageStore(textureOutput, pixel_coords, vec4(0.0, 0.0,1.0, 1.0));
  }
  
  float t = intersecCircle(s, r);
  
  if (t != -1) {
      imageStore(textureOutput, pixel_coords, ray_color(r, t));
      return;
  }
  
  
  //imageStore(textureOutput, pixel_coords, vec4(0.0, 1.0, 0.0, 1.0));
}
