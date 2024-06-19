#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTexture;
    float shininess;
};

uniform Material material;

in vec2 TexCoords;

out vec4 color;

void main()
{
    vec3 texColor = texture(material.diffuseTexture, TexCoords).rgb;
    color = vec4(texColor, 1.0);
}