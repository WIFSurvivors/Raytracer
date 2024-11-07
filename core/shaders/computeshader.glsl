#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D textureOutput;

uniform vec2 mousePos;


bool is_in_circle(vec2 p){
	ivec2 dims = imageSize(textureOutput);
    float a= mousePos.x / dims.x;
    float b= mousePos.y / dims.y;
    
    float r = 0.25;
    
    return (p.x - a)*(p.x - a) + (p.y- b)*(p.y- b) <= r*r;
    
}

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
	ivec2 dims = imageSize(textureOutput);
	vec4 pixelColor = vec4(1.0, 1.0, 0.0, 1.0);
	if(is_in_circle(vec2(float(pixelCoord.x)/float(dims.x), float(pixelCoord.y)/float(dims.y)))) {

		pixelColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
    

	// if(pixelCoord.y < 100) {
	//	pixelColor = vec4(1.0,0.0,0.0,1.0);
	// }
    imageStore(textureOutput, pixelCoord, pixelColor);
}
