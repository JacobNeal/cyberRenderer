#version 330 core

layout (location=0) in vec3 inPos;
layout (location=1) in vec2 inText;
layout (location=2) in vec3 inNorm;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragPosition = vec3(model * vec4(inPos, 1.0));
    fragNormal   = mat3(transpose(inverse(model))) * inNorm;
    fragTexCoord = inText;

    gl_Position = projection * view * vec4(fragPosition, 1.0);
}
