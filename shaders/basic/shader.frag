#version 410 core

out vec4 frag_color;

void main() {
	vec3 color = vec3(1.0, 0.0, 0.0);
	frag_color = vec4 (color, 1.0);
}
