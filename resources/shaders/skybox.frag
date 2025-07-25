#version 430 core

in VS_OUT {
    vec3 texCoords;
} vs_in;

out vec4 oFragColor;

uniform samplerCube uCubeMap;

void main() {
    oFragColor = texture(uCubeMap, vs_in.texCoords);
}
