#version 430 core

out vec3 oColor;

void main() {
    vec2 positions[6] = vec2[](
        vec2(-1.0,  1.0),
        vec2( 1.0,  1.0),
        vec2(-1.0, -1.0),

        vec2(-1.0, -1.0),
        vec2( 1.0,  1.0),
        vec2( 1.0, -1.0)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);

    float lerpFactor = (positions[gl_VertexID].y + 1.0) / 2.0;
    oColor = mix(vec3(0.4, 0.4, 0.4), vec3(0.2, 0.4, 1.0), lerpFactor);
}
