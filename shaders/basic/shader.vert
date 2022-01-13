#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform mat4 rotation;

void main()
{
    fTexCoords = vTexCoords;

    float scale = 0.5;
    gl_Position = rotation * vec4(vPosition * scale, 1.0);
}