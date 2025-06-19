#version 430 core

const int BLOCKS = 1000;
const int VECTOR_SIZE = int(round(pow(BLOCKS, 1.0 / 3.0)));

layout(location = 0) in vec3 aPosition;

out vec4 oColor;

uniform mat4 uView;
uniform mat4 uProjection;

layout(binding = 1, std430) readonly buffer colors {
    ivec4 color[];
};

void main() {
    ivec4 c = color[gl_InstanceID];
    oColor = vec4(c.r, c.g, c.b, c.a) / 255.0;

    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x), float(y), float(z));
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);

    gl_Position = uProjection * uView * worldPos;
}
