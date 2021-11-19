#version 400

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 textcoord;

out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    colour = vertexNormal;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
