#version 430 core

in vec3 ioTexCoords;

out vec4 FragColor;

void main() {
    FragColor = vec4(abs(normalize(ioTexCoords)), 1.0);
}
