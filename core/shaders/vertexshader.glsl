#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
void main()
{
  vec2 TexCoords = aTexCoords;
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
