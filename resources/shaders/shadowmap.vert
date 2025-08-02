#version 430 core

const int VECTOR_SIZE = 10;
const int BLOCKS = VECTOR_SIZE * VECTOR_SIZE * VECTOR_SIZE;

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProj;

void main(void) {
    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x) + 0.5, float(y) + 0.5, float(z) + 0.5);
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);
    gl_Position = uViewProj * worldPos;
}
