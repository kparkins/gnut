#version 330 core

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;
uniform float ambient_intensity;

uniform float sin_time;
uniform bool shader_demo;
uniform bool use_pcf;
uniform bool use_texture;
uniform sampler2D texture_sampler;
uniform sampler2D shadow_sampler;
uniform samplerCube environment_sampler;

in vec3 fragment_normal;
in vec3 fragment_position;
in vec2 fragment_texcoords;
in vec4 fragment_lightposition;

out vec4 final_color;

float kernel[25] = float[](1, 4, 7, 5, 1,
                      4, 16, 26, 16, 4,
                      7, 26, 41, 26, 7,
                      4, 16, 26, 16, 4,
                      1, 4, 7, 4, 1);

void main() {
    vec3 fragment_color;

    if(!use_texture) {
        vec3 rvec = reflect(normalize(fragment_position - camera_position), normalize(fragment_normal));
        fragment_color = texture(environment_sampler, rvec).rgb;
    } else if(shader_demo){
        float time = ((sin_time + 1) / 2.0);
        vec3 rgb = ((normalize(fragment_normal) + 1.0) / 2.0);
        rgb.x += (1.0 - rgb.x - rgb.x) * time;
        rgb.y += (1.0 - rgb.y - rgb.y) * time;
        rgb.z += (1.0 - rgb.z - rgb.z) * time;
        fragment_color = rgb;
    } else {
        fragment_color = texture(texture_sampler, fragment_texcoords).rgb;
    }
    vec3 ambient = ambient_intensity * fragment_color;

    float light_angle = dot(normalize(fragment_normal), normalize(vec3(light_position) - fragment_position));
    float diffuse_intensity = max(light_angle,0);

    vec3 diffuse = diffuse_intensity * light_color;

    vec3 frag_depthcoords =  (fragment_lightposition.xyz / fragment_lightposition.w) * .5 + .5;
    float frag_nearest = texture(shadow_sampler, frag_depthcoords.xy).r;
    float frag_current = frag_depthcoords.z;

    float shadow_contribution = 0.0;
    float shadow_bias = max(0.04 * (1.0 - light_angle), .005);
    if(use_pcf) {
        vec2 offset;
        vec2 step = 1.0 / textureSize(shadow_sampler, 0);
        for(int i = -2; i <= 2; ++i) {
            for(int j = -2; j <= 2; ++j) {
                offset = vec2(i,j) * step;
                float d = texture(shadow_sampler, frag_depthcoords.xy + offset).r;
                if(d < frag_current - shadow_bias  && frag_current <= 1.0) {
                    float weight = kernel[(i + 2) * 5 + (j + 2)];
                    shadow_contribution += .5 * weight;
                }
            }
        }
        shadow_contribution /= 273.0;
    } else if(frag_nearest < frag_current - shadow_bias && frag_current <= 1.0) {
        shadow_contribution = 0.5;
    }


    final_color = vec4(((1.0 - shadow_contribution) * diffuse + ambient) * fragment_color, 1.f);
}