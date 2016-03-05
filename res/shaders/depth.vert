#version 330 core

layout (location = 0) in vec3 vertex_position;

uniform mat4 model;
uniform mat4 light;

void main() {
    gl_Position = light * model * vec4(vertex_position,1);
}