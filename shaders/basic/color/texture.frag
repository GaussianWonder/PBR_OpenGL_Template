#version 410 core

in vec2 fTexCoords;

uniform sampler2D diffuseTexture;

out vec4 FragColor;

void main()
{
  vec3 color = texture(diffuseTexture, fTexCoords).rgb;
  FragColor = vec4(color, 1.0f);
}