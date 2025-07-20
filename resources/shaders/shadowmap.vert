#version 430 core

const int BLOCKS = 1000;
const int VECTOR_SIZE = int(round(pow(BLOCKS, 1.0 / 3.0)));

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(binding = 5, std430) readonly buffer colors {
    ivec4 color[];
};

uniform mat4 uView;
uniform mat4 uProjection;

void main(void) {
    if (color[gl_InstanceID].a == 0.0) {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
        return;
    }

    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x), float(y), float(z));
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);
    gl_Position = uProjection * uView * worldPos;
}
