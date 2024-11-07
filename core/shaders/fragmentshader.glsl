#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D text;

void main() {
	vec3 texCol = texture(text, TexCoords).rgb;
	FragColor = vec4(texCol,1.0f);
	
} 
