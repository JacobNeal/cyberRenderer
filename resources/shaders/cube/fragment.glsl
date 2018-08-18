#version 330 core

in vec3 fragColor;
in vec3 fragNorm;

out vec4 FragColor;

void main()
{
   FragColor = vec4(fragColor, 1.0);
}