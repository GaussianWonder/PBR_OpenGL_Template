#version 410 core

// Positions/Coordinates
layout (location = 0) in vec3 vPosition;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 vNormal;
// Texture Coordinates
layout (location = 2) in vec2 vTexCoords;

out mat4 gModel;
out vec2 gTexCoords;
out vec3 gNormal;

out mat4 gProjection;
out mat4 gView;
out vec3 gCamPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 cameraPosition;

void main()
{
  gTexCoords = vTexCoords;
  gNormal = vNormal;
  gModel = model;

  gProjection = projection;
  gView = view;
  gCamPos = cameraPosition;

	gl_Position = /* projection * view * model * */ vec4(vPosition, 1.0);
}
