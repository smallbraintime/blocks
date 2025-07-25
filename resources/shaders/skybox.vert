#version 430 core

out VS_OUT {
    vec3 texCoords;
} vs_out;

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProj;

void main() {
    vec4 pos = uViewProj * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
    vs_out.texCoords = aPosition;
}
