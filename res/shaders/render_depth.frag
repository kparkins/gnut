#version 330 core

in vec2 fragment_texcoords;

uniform sampler2D texture_sampler;

out vec4 fragment_color;

void main() {
    float value = texture(texture_sampler, fragment_texcoords).r;
    fragment_color = vec4(value, value, value, 1);
}
