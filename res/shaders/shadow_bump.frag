#version 330 core

uniform vec3 light_color;
uniform vec3 light_position;
uniform float ambient_intensity;
uniform bool use_normal_map;

uniform sampler2D texture_sampler;
uniform sampler2D shadow_sampler;
uniform sampler2D normal_sampler;

in vec3 fragment_normal;
in vec3 fragment_position;
in vec2 fragment_texcoords;
in vec4 fragment_lightposition;
in mat3 tbn;

out vec4 final_color;

void main() {

    vec3 fragment_color = texture(texture_sampler, fragment_texcoords).rgb;
    vec3 ambient = ambient_intensity * fragment_color;

    vec3 normal;
    if(use_normal_map) {
        normal = texture(normal_sampler, fragment_texcoords).rgb * 2.0 - 1.0;
        normal = normalize(tbn * normalize(normal));
    } else {
        normal = fragment_normal;
    }
    float light_angle = dot(normalize(normal), normalize(vec3(light_position) - fragment_position));
    float diffuse_intensity = max(light_angle,0);

    vec3 diffuse = diffuse_intensity * light_color;

    vec3 frag_depthcoords =  (fragment_lightposition.xyz / fragment_lightposition.w) * .5 + .5;
    float frag_nearest = texture(shadow_sampler, frag_depthcoords.xy).r;
    float frag_current = frag_depthcoords.z;

    float shadow_bias = max(0.04 * (1.0 - light_angle), .005);
    float shadow_contribution;
    if(frag_nearest < frag_current - shadow_bias && frag_current <= 1.0) {
        shadow_contribution = .5;
    } else {
        shadow_contribution = 0.0;
    }


    final_color = vec4(((1.0 - shadow_contribution) * diffuse + ambient) * fragment_color, 1.f);
}
