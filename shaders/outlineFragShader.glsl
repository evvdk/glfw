#version 330 core
out vec4 FragColor;
uniform float uTime;
in vec3 pos;

void main() {
    float h = clamp(pos.y / 5.f, 0.0, 1.0);

    vec3 lowColor = vec3(1.0, 0.0, 0.0);
    vec3 highColor = vec3(0.55, 0.81, 0.89);

    vec3 color = mix(lowColor, highColor, h);
    FragColor = vec4(color, 0.3f);
}