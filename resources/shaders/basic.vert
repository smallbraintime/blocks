#version 430 core

const int VECTOR_SIZE = 10;
const int BLOCKS = VECTOR_SIZE * VECTOR_SIZE * VECTOR_SIZE;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(binding = 5, std430) readonly buffer colors {
    ivec4 color[];
};

uniform mat4 viewProj;
uniform int uIndex;

void main() {
    int x = uIndex % VECTOR_SIZE;
    int y = (uIndex / VECTOR_SIZE) % VECTOR_SIZE;
    int z = uIndex / (VECTOR_SIZE * VECTOR_SIZE);

    vec3 blockPos = vec3(x + 0.5, y + 0.5, z + 0.5);
    vec3 worldPos = aPosition + blockPos;

    gl_Position = viewProj * vec4(worldPos, 1.0);
}
