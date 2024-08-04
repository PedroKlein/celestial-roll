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
    float time;
    vec3 viewPos;
};

uniform Material material;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

void main()
{
    vec3 texColor = texture(material.diffuseTexture, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < lightCount; i++) {
        vec3 lightPos = lights[i].position.xyz;
        float lightIntensity = lights[i].color.a;
        vec3 lightColor = lights[i].color.rgb * lightIntensity;

        // Calculate distance and attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        lightColor *= attenuation;

        // Ambient
        vec3 ambient = material.ambient * lightColor;

        // Diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = material.diffuse * diff * lightColor;

        // Specular
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = material.specular * spec * lightColor;

        result += ambient + diffuse + specular;
    }

    color = vec4(result * texColor, 1.0);
}
