#version 430 core

const int BLOCKS = 1000;
const int VECTOR_SIZE = int(round(pow(BLOCKS, 1.0 / 3.0)));

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProj;

void main(void) {
    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x), float(y), float(z));
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);
    gl_Position = uViewProj * worldPos;
}
