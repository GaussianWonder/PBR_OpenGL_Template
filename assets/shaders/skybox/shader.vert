#version 410 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec4 tempPos = projection * view * vec4(vPosition, 1.0);
  fTexCoords = vPosition;
  gl_Position = tempPos.xyww;
}
