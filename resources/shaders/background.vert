#version 430 core

out vec3 ioTexCoords;

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

void main() {
    vec4 pos = uProjection * uView * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
    ioTexCoords = aPosition;
}
