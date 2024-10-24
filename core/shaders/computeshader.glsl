#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

void main(){
  ivec2 pixel_coords =  ivec2(gl_GlobalInvocationID.xy);
  vec4  blue = vec4(0.0,0.0,1.0,1.0);
  imageStore(textureOutput, pixel_coords,blue);
}
