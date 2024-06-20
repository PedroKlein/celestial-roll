#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * aPos;
    TexCoords = aTexCoords; 
    FragPos = vec3(model * aPos);
    
    Normal = normalize((mat3(transpose(inverse(model))) * vec3(aNormal)));
}

