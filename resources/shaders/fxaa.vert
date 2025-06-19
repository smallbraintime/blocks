#version 430 core

out vec4 oColor;

void main(void) {
    vec2 positions[3] = vec2[](
        vec2(-1.0,  1.5),
        vec2( 1.5,  -1.0),
        vec2(-1.0, -1.0)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
