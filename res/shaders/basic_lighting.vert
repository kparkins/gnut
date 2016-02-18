#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragment_normal;
out vec3 fragment_position;

void main() {
    fragment_normal = vec3(model * vec4(vertex_normal,1));
    fragment_position = vec3(model * vec4(vertex_position, 1));
    gl_Position = projection * view * model * vec4(vertex_position, 1);
}