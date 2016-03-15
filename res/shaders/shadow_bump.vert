#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoords;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_matrix;

out vec3 fragment_normal;
out vec3 fragment_position;
out vec2 fragment_texcoords;
out vec4 fragment_lightposition;
out mat3 tbn;

void main() {
    fragment_normal = vec3(model* vec4(vertex_normal, 0.0));
    fragment_position = vec3(model * vec4(vertex_position, 1));
    fragment_texcoords = vertex_texcoords;
    fragment_lightposition = light_matrix * vec4(fragment_position, 1);
    gl_Position = projection * view * model * vec4(vertex_position, 1);

    mat3 m = mat3(model);
    vec3 t = normalize(m * vertex_tangent);
    vec3 b = normalize(m * vertex_bitangent);
    vec3 n = normalize(m * vertex_normal);

    tbn = mat3(t, b, n);

}
