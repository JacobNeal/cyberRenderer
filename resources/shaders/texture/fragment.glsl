#version 330 core

in vec2 fragTexCoord;

out vec3 color;

uniform sampler2D text;


void main()
{
    float dx = 3.*(1./800.);
    float dy = 3.*(1./640.);
    vec2 coord = vec2(dx * floor(fragTexCoord.x / dx), dy * floor(fragTexCoord.y / dy));
    color = texture(text, coord).xyz;
}