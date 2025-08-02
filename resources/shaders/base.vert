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

out VS_OUT {
    vec4 color;
    vec3 fragPos;
    vec3 cameraPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 fragPosLightSpace;
    vec3 lightPos;
} vs_out;

uniform mat4 uViewProj;
uniform vec3 uCameraPos;
uniform mat4 uLightViewProj;
uniform vec3 uLightPos;

void main() {
    if (color[gl_InstanceID].a == 0.0) {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
        return;
    }

    ivec4 c = color[gl_InstanceID];
    vs_out.color = vec4(c.r, c.g, c.b, c.a) / 255.0;

    int z = gl_InstanceID / (VECTOR_SIZE * VECTOR_SIZE);
    int r = gl_InstanceID % (VECTOR_SIZE * VECTOR_SIZE);
    int y = r / VECTOR_SIZE;
    int x = r % VECTOR_SIZE;
    vec3 modelPos = vec3(float(x) + 0.5, float(y) + 0.5, float(z) + 0.5);
    vec4 worldPos = vec4(aPosition + modelPos, 1.0);

    gl_Position = uViewProj * worldPos;

    vs_out.fragPos = vec3(worldPos.xyz);
    vs_out.cameraPos = uCameraPos;
    vs_out.texCoord = aTexCoord;
    vs_out.normal = aNormal;
    vs_out.tangent = aTangent;
    vs_out.bitangent = aBitangent;
    vs_out.lightPos = uLightPos;
    vs_out.fragPosLightSpace = uLightViewProj * worldPos;
}
