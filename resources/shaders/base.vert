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

out vec4 ioColor;
out vec3 ioFragPos;
out vec3 ioViewPos;
out vec2 ioTexCoord;
out vec3 ioNormal;
out vec3 ioTangent;
out vec3 ioBitangent;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uViewPos;

void main() {
    ivec4 c = color[gl_InstanceID];
    ioColor = vec4(c.r, c.g, c.b, c.a) / 255.0;

    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x), float(y), float(z));
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);

    ioFragPos = vec3(worldPos.xyz);
    ioViewPos = uViewPos;
    ioTexCoord = aTexCoord;
    ioNormal = aNormal;
    ioTangent = aTangent;
    ioBitangent = aBitangent;

    gl_Position = uProjection * uView * worldPos;
}
