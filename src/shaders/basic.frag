#version 330 core

out vec4 FragColor;
in vec3 ourColor;

uniform float mulx;
uniform float muly;
uniform float mulz;

void main()
{
    FragColor = vec4(ourColor.x * mulx, ourColor.y * muly, ourColor.z * mulz, 1.0);
}
