#version 330 core

uniform vec3 light_color;
uniform vec3 light_position;
uniform float ambient_intensity;

uniform sampler2D texture_sampler;

in vec3 fragment_normal;
in vec3 fragment_position;
in vec2 fragment_texcoords;

out vec4 final_color;

void main() {
    vec3 fragment_color = texture(texture_sampler, fragment_texcoords).rgb;
    vec3 ambient = ambient_intensity * fragment_color;

    float diffuse_intensity;
    diffuse_intensity = dot(normalize(fragment_normal), normalize(vec3(light_position) - fragment_position));
    diffuse_intensity = max(diffuse_intensity,0);

    vec3 diffuse = diffuse_intensity * light_color;

    final_color = vec4((diffuse + ambient) * fragment_color, 1.f);
}