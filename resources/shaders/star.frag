#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

layout(std140) uniform Common {
    float time;
    vec3 viewPos;
};

float noise(vec3 pos) {
    float t = dot(pos, vec3(12.9898, 78.233, 29.145));
    return fract(sin(t) * 43758.5453);
}

void main() {
    float intensity = noise(normalize(FragPos) * time);
    vec3 color = vec3(1.0, 0.0, 0.0) * intensity;

    float dist = length(FragPos - viewPos);
    float corona = exp(-dist * 10.0) * intensity;
    color += corona * vec3(1.0, 0.7, 0.4);

    FragColor = vec4(color, 1.0);
}
