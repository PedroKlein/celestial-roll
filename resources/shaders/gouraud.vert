#version 330 core

#define MAX_LIGHTS 10

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTexture;
    float shininess;
};

struct Light {
    vec4 position;
    vec4 color;  // RGBA where A is intensity
    float constant;
    float linear;
    float quadratic;
};

layout(std140) uniform Lights {
    int lightCount;
    Light lights[MAX_LIGHTS];
};

layout(std140) uniform Common {
    vec3 viewPos;
};

uniform Material material;

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 Tex;
out vec3 Vcolor;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

void main() {
    vec3 FragPos = vec3(model * aPos);
    vec3 Normal = normalize(mat3(transpose(inverse(model))) * vec3(aNormal));
    vec3 result = vec3(0.0);

    for (int i = 0; i < lightCount; i++) {
        vec3 lightPos = lights[i].position.xyz;
        float lightIntensity = lights[i].color.a;
        vec3 lightColor = lights[i].color.rgb * lightIntensity;

        vec3 lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        lightColor *= attenuation;

        // Ambient
        vec3 ambient = material.ambient * lightColor;

        // Diffuse (Lambert)
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = material.diffuse * lightColor * diff;

        result += ambient + diffuse;
    }

    Tex = aTexCoords;
    Vcolor = result;
    gl_Position = projection * view * model * aPos;
}
