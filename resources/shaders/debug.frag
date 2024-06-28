#version 330 core

#define MAX_LIGHTS 10

in vec3 oColor;
out vec4 FragColor;

struct Light {
    vec4 position;
    vec4 color;
};

layout(std140) uniform Lights {
    int lightCount;
    Light lights[MAX_LIGHTS];
};

layout(std140) uniform ViewPos {
    vec3 viewPos;
};

void main()
{
    FragColor = vec4(oColor, 1.0);
}