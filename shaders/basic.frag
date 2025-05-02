#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform float mulx;
uniform float muly;
uniform float mulz;
uniform sampler2D ourTexture;
uniform bool isTextured;

void main() {
    if (isTextured) {
        FragColor = texture(ourTexture, texCoord);
        FragColor.a *= 0.25;
    } else {
        FragColor = vec4(ourColor.x * mulx, ourColor.y * muly, ourColor.z * mulz, 128);
    }
}
