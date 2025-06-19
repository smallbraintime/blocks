#version 430 core

in vec4 oColor;

out vec4 FragColor;

void main(void) {
    discard;
    FragColor = oColor;
}
