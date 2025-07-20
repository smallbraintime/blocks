#version 430 core

const vec2 verts[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

out vec2 vUV;

void main() {
    gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
    vUV = (verts[gl_VertexID] + 1.0) * 0.5;
}
