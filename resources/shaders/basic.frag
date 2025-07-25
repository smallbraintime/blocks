#version 430 core

out vec4 oFragColor;

uniform vec3 uColor;

void main() {
    oFragColor = vec4(vec3(uColor), 1.0);
}
