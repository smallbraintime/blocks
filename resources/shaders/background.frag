#version 430 core

in vec3 ioTexCoords;
out vec4 FragColor;

uniform samplerCube uCubeMap;

void main() {
    FragColor = texture(uCubeMap, ioTexCoords);
}
