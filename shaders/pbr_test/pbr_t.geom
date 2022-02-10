#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in mat4 gModel[];
in vec2 gTexCoords[];
in vec3 gNormal[];

in mat4 gProjection[];
in mat4 gView[];
in vec3 gCamPos[];

out vec2 fTexCoords;
out vec3 fNormal;

out vec3 fPosition;
out vec3 fLightPosition;
out vec3 fLightColor;
out vec3 fCamPos;
out mat3 fTBN;

void main()
{
  // TODO proper lights maybe?
  vec3 gLightPosition = vec3(-1.0, 1.0, -2.0);
  vec3 gLightColor = vec3(1.0, 0.988, 0.902);
  // Edges of the triangle
  vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  // Lengths of UV differences
  vec2 deltaUV0 = gTexCoords[1] - gTexCoords[0];
  vec2 deltaUV1 = gTexCoords[2] - gTexCoords[0];

  // one over the determinant
  float invDet = 1.0 / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

  vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
  vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

  vec3 T = normalize(vec3(gModel[0] * vec4(tangent, 0.0)));
  vec3 B = normalize(vec3(gModel[0] * vec4(bitangent, 0.0)));
  vec3 N = normalize(vec3(gModel[0] * vec4(cross(edge1, edge0), 0.0)));

  mat3 TBN = mat3(T, B, N);
  fTBN = TBN;
  // TBN is an orthogonal matrix and so its inverse is equal to its transpose
  TBN = transpose(TBN);

  for (int i=0; i<3; ++i)
  {
    vec4 pos = gModel[i] * gl_in[i].gl_Position;
    gl_Position = gProjection[i] * gView[i] * pos;
    fNormal = gNormal[i];
    fTexCoords = gTexCoords[i];
    // Change all lighting variables to TBN space
    fPosition = pos.xyz;
    fLightPosition = gLightPosition;
    fLightColor = gLightColor;
    fCamPos = gCamPos[i];
    EmitVertex();
  }

  EndPrimitive();
}