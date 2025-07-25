#version 430 core

in vec2 vUV;

out vec4 oFragColor;

layout(binding = 0) uniform sampler2D depthTexture;

void main() {
    float depth = texture(depthTexture, vUV).r;
    oFragColor = vec4(vec3(depth), 1.0);
}
