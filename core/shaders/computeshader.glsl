#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

uniform float time;

void main(){
  ivec2 pixel_coords =  ivec2(gl_GlobalInvocationID.xy);
  vec4  blue = vec4(0.0,0.0,1.0,1.0);
  vec4 purple = vec4(1.0,0.0,1.0,1.0);
  
  // Increase the speed of oscillation by multiplying time by a factor
    float timeFactor = sin(time * 5.0 + pixel_coords.x * 0.1); // Faster oscillation
    
    // Make transitions sharper by using abs() and increasing the range of values
    float pronouncedFactor = abs(sin(time * 5.0 + pixel_coords.x * 0.2)); // Faster and sharper
    
    // Adjust color blending to be more pronounced (exaggerate the difference)
    vec4 animatedBlue = mix(blue, purple, pronouncedFactor); // Switches between blue and purple
    vec4 animatedPurple = mix(purple, blue, pronouncedFactor); // Reverse

  if(pixel_coords.x % 2 == 0) {

    imageStore(textureOutput, pixel_coords,animatedBlue);
  }else {
    
    imageStore(textureOutput, pixel_coords,animatedPurple);
  }
}
