#version 330 core

layout (location=0) in vec3 inPos;

out vec2 fragTexCoord;

void main()
{
    gl_Position  = vec4(inPos, 1.0);
    fragTexCoord = (inPos.xy + vec2(1, 1)) / 2.0;
}
