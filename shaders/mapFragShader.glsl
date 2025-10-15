#version 330 core
out vec4 FragColor;
uniform float uTime;
in vec3 pos;

void main() {
    float h = clamp(pos.y / 5.f, 0.0, 1.0);

    vec3 lowColor = vec3(0.102, 0.4078, 0.502);
    vec3 highColor = vec3(0.0431, 0.0196, 0.3686);

    vec3 color = mix(lowColor, highColor, h);

    FragColor = vec4(color, 1.0);
}