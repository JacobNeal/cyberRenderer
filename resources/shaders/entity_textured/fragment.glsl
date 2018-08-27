#version 330 core

layout (location=0) out vec3 color;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPosition;

out vec4 FragColor;

uniform sampler2D text;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
    float ambientStrength  = 0.35;
    float specularStrength = 0.5;

    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 unitNormal     = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 diffuse        = max(dot(unitNormal, lightDirection), 0.0) * lightColor;

    // Specular lighting
    vec3 cameraDirection     = normalize(cameraPosition - fragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, unitNormal);
    vec3 specular            = specularStrength * pow(max(dot(cameraDirection, reflectionDirection), 0.0), 8) * lightColor;

    // Calculate fragment color
    //FragColor = (vec4(ambient, 1.0) + 
    //             vec4(diffuse, 1.0) + 
    //             vec4(specular, 1.0)) * 
    //            texture(text, fragTexCoord);

    FragColor = texture(text, fragTexCoord);
}
