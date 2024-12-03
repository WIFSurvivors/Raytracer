#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

// Model does not really exist rn so bleh
uniform float time;
uniform vec3 cameraPos;
// uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;
uniform vec3 v0_no_use;
uniform vec3 v1_no_use;
uniform vec3 v2_no_use;
uniform vec2 imageSize_no_use;

const int hittableCount = 3;
const int emitterCount = 1;

struct Sphere {
    float r;
    vec3 C;
    vec3 color;
    float reflectivity; //maybe [0,1] 0 not reflective 1 absolute reflective
    // float refraction; ??
    // float emissive;?
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

struct Light {
    vec3 position; // Position for point light
    vec3 direction; // Direction for directional light
    vec3 color; // Light color (RGB)
    float intensity; // Light intensity
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
    vec3 r = vec3(rand(n.xy), rand(n.yz), rand(n.xz) + time);
    if (dot(r, n) > 0) {
        return r;
    } else {
        return -r;
    }
}


bool isInShadow(Sphere s[hittableCount], Ray r, int originObject, float distance){
  for(int i = 0; i < hittableCount; i++){
    if (i == originObject){ continue;}
    float tShadow = intersectsSphere(s[i],r);
    if(tShadow > 0.0 && tShadow < distance){
      return true;
    }

  }

  return false;
}

vec4 CalcColorWithLightSources(Sphere s[hittableCount], Ray r, Light emitter[emitterCount]) {
    //Maximal Bounces for one ray
    int MAXIMAL_BOUNCES = 10;
    // Color is to the start BLACK because no light reached a pixel yet
    vec3 color = vec3(0.0, 0.0, 0.0);
    vec3 reflec_accumulation = vec3(1.0, 1.0, 1.0); //Reflection in percent 100% red 100% green and blue

    //For each bounce we want:
    for (int bounce = 0; bounce <= MAXIMAL_BOUNCES; bounce++) {
        float t = 1.0 / 0.0; // =)
        int index = -1;
        //To determine the first object we hit from camera to pixel;
        for (int i = 0; i < hittableCount; i++) {
            float temp = intersectsSphere(s[i], r);
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
            vec3 N = normalize(sectionPoint - s[index].C);
            // For each lightsource we want;
            for(int lIndex = 0; lIndex < emitterCount; lIndex++){
              Light light = emitter[lIndex];
              // To calculate the shadow ray to the light source;
              vec3 shadowRay = normalize(light.position - sectionPoint);
              //the distance to the light to the section point;
              float distanceToLight = length(light.position - sectionPoint);
              //The attentuation based on Intensity = 1 / d^2
              float attenuation = 1.0 / (distanceToLight * distanceToLight);  // Inverse square law
              // Check if the shadow ray intersects other objects in its path
              bool isShadow = isInShadow(s,Ray(sectionPoint, shadowRay), index, distanceToLight);
              // If no intersections, apply direct lighting (diffuse lighting)
              if (!isShadow) {
                  vec3 reflecDirection = reflect(r.direction,N);
                  sectionPoint = sectionPoint + 0.01 * reflecDirection;
                  r = Ray(sectionPoint, reflecDirection);
                  float diffuse = max(dot(N, shadowRay), 0.0);  // Lambertian diffuse shading
                  vec3 lighting = reflec_accumulation * light.color * s[index].color * light.intensity  * diffuse * attenuation;
                  reflec_accumulation *= s[index].reflectivity;
                  color += lighting;
                  if (length(reflec_accumulation) < 0.01) break; // If accumulation reach a certain point there should be no further reflections and the bounces should stop
              }
            }

        }
    }
    return vec4(color,1.0);
}

vec4 CalcColor(Sphere s[hittableCount], Ray r) {
    int MAXIMAL_BOUNCES = 100;
    vec3 color = vec3(0.0, 0.0, 0.0);
    vec3 reflec_accumulation = vec3(1.0, 1.0, 1.0); //Reflection in percent 100% red 100% green and blue

    for (int bounce = 0; bounce <= MAXIMAL_BOUNCES; bounce++) {
        float t = 1.0 / 0.0; // =)
        int index = -1;

        for (int i = 0; i < hittableCount; i++) {
            float temp = intersectsSphere(s[i], r);
            if (temp < t && temp > 0.0) {
                t = temp;
                index = i;
            }
        }

        if (t > 0.0 && index >= 0) {
            vec3 sectionPoint = r.origin + t * r.direction;
            vec3 N = normalize(sectionPoint - s[index].C);
            //vec3 dirT = randOnHemisphere(N);
            vec3 dir = reflect(r.direction, N);
            sectionPoint = sectionPoint + 0.01 * dir; //Move a little further to prevent intersection with current sphere

            r = Ray(sectionPoint, dir);
            color += reflec_accumulation * s[index].color * max(dot(N, dir), 0.0); // acc * color. Meaning color will always be displayed at first hit. afterwards we get little color for the further hits because
            reflec_accumulation *= s[index].reflectivity; //  accumulation is beeing reduced by the reflectivity of the current hit.
            if (length(reflec_accumulation) < 0.01) break; // If accumulation reach a certain point there should be no further reflections and the bounces should stop
        } else {
            vec3 N = normalize(r.direction);
            float a = 0.5 * (N.y + 1.0);
            vec3 res = (1.0 - a) * vec3(0.0, 0.0, 0.0) + a * vec3(0.5, 0.7, 1.0);
            color += vec3(0.0, 0.0, 0.0) * reflec_accumulation;
            break; // missed
        }
    }
    return vec4(color, 1.0);
}

vec4 rayColor(Ray r) {
    //mat4 MVP = Projection * View * Model * mat4(1.0);
    vec3 v0 = vec3(-0.6, -0.8, -0.5);
    vec3 v1 = vec3(0.6, -0.8, -0.5);
    vec3 v2 = vec3(0.0, 0.8, 0.2);

    Vertex v = Vertex(v0, v1, v2);

    vec4 c1 = vec4(-4.0, 0.0, -1.0, 1.0);
    vec4 c2 = vec4(4.0, 0.0, -1.0, 1.0);
    vec4 c3 = vec4(0.0, -10.0, 0.0, 1.0);

    Sphere s1 = Sphere(3.9, c1.xyz, vec3(0.0, 1.0, 0.0), 1.0);
    Sphere s2 = Sphere(3.9, c2.xyz, vec3(0.5, 0.0, 0.5), 0.2);
    Sphere s3 = Sphere(5.0, c3.xyz, vec3(1.0, 1.0, 1.0), 0.5);

    Sphere s[3];
    s[0] = s1;
    s[1] = s2;
    s[2] = s3;
    float t = -1.0;
    float temp = -1.0;

    Light[1] lightSources;
    lightSources[0] = Light(vec3(0.0,10.0,10.0),vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0),100.0);
    //return CalcColor(s, r);
    return CalcColorWithLightSources(s,r,lightSources);
    //t = intersectsSphere(s1, r); //
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
    /*
                            if (t > 0.0) {
                                vec3 N = normalize(r.origin + t * r.direction - s1.C);
                                vec3 dir = randOnHemisphere(N);
                                return vec4(dir, 1.0);
                            }

                            t = intersectsSphere(s2, r);
                            if (t > 0.0) {
                                vec3 N = normalize(r.origin + t * r.direction - s2.C);
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
                            */
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

    // vec3 rayOrigin = vec3(0.0, 10.0, 10.0); // Ray origin should always be camera position. Current camera position is not used
    vec3 rayOrigin = cameraPos;

    //ray in clip
    vec4 rayPixel = vec4(x, y, -1.0, 1.0);

    // Convert to eye space
	//mat4 Projection = mat4(1.0);
	//mat4 View = mat4(1.0);
    vec4 rayEye = inverse(Projection) * rayPixel;
    rayEye = vec4(rayEye.xy, -1.0, 0.0); // We = 0 dir

    // Convert to world space
    vec3 rayDirWorld = normalize(vec3(inverse(View) * rayEye));

    //vec3 rayDirection = normalize(rayPixel.xyz - rayOrigin);
    Ray r = Ray(rayOrigin, rayDirWorld);
    //imageStore(textureOutput, pixelCoords, vec4(rayDirWorld, 1.0));
    imageStore(textureOutput, pixelCoords, rayColor(r));
}
