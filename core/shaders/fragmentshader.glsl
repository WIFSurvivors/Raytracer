#version 430 core
out vec4 FragColor;
in vec2 tex;

uniform sampler2D text;

void main()
{
  vec3 texCol = texture(text,tex).rgb;
  FragColor = vec4(texCol,1.0f);
} 
