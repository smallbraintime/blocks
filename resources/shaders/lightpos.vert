#version 430 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProj;
uniform vec3 uLightPos;

void main(void) {
    vec3 worldPos = aPosition + uLightPos;
    gl_Position = uViewProj * vec4(worldPos, 1.0);
}
