#version 430 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D depthTexture;

void main() {
    float depth = texture(depthTexture, vUV).r;
    fragColor = vec4(vec3(depth), 1.0);
}
