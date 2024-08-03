#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 rotation;

void main() {
    TexCoords = aPos;
    // Remove the translation component from the view matrix
    mat4 viewNoTranslation = mat4(mat3(view));
    vec4 pos = projection * viewNoTranslation * rotation * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // A trick to maintain z-value at maximum depth
}