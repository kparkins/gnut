#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragment_normal;
out vec3 fragment_position;
out vec3 fragment_color;

void main() {
    fragment_color = vertex_color;
    fragment_normal = mat3(model) * vertex_normal;
    fragment_position = vec3(model * vec4(vertex_position, 1));
    gl_Position = projection * view * model * vec4(vertex_position, 1);
}
