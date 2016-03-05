#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoords;

out vec2 fragment_texcoords;

void main() {
    gl_Position = vec4(vertex_position,1);
    fragment_texcoords = vertex_texcoords;
}