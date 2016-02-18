#version 330 core

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 fragment_color;
uniform float ambient_intensity;

in vec3 fragment_normal;
in vec3 fragment_position;

out vec4 final_color;

void main() {
    vec3 ambient = ambient_intensity * light_color;

    float diffuse_intensity = dot(normalize(fragment_normal), normalize(light_position - fragment_position));
    diffuse_intensity = abs(diffuse_intensity);
    vec3 diffuse = diffuse_intensity * light_color;

    final_color = vec4((diffuse + ambient) * fragment_color, 1.f);
}