#version 430 core

layout(location = 0) in vec3 aPosition;

out vec4 oColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout(binding = 1, std430) readonly buffer colors {
    vec4 color[];
};

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    // TODO: add calculating pos based on fixed buffer
}
