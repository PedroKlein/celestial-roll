#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 oColor;

uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    oColor = aColor;
}
