#version 430 core

in vec4 oColor;

out vec4 FragColor;

void main() {
    // if (oColor.a == 0) {
    //     discard;
    // }

    // TODO: add lighting
    FragColor = oColor;
}
