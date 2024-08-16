#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTexture;
    float shininess;
};

uniform Material material;

in vec2 Tex;
in vec3 Vcolor;

out vec4 FragColor;

void main() {
    vec3 texColor = texture(material.diffuseTexture, Tex).rgb;
    FragColor = vec4(Vcolor * texColor, 1.0);
}