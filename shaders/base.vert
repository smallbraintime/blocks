#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec4 oColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec4 uColor;

readonly restrict layout(std430) buffer uInstanceData {
    struct Instance {
        mat4 model;
        vec4 color;
    };
    Instance instances[];
};

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    oColor = uColor * aColor;
}
