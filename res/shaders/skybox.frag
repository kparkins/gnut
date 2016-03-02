#version 330 core

in vec3 texcoord;

out vec4 color;

uniform samplerCube skybox_sampler;

void main() {
	color = texture(skybox_sampler, texcoord);
}
