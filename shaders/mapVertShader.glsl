#version 330 core
layout (location = 0) in vec3 aPos;

#ifdef GL_ES
    precision mediump float;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTime;
uniform vec3 cameraPos;

out vec3 pos;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(43.232, 75.876)))*4526.3257);
}

void main() {
    pos = aPos;
    vec3 wave = vec3(hash(pos.xy + (sin(uTime * 0.00001f)+1)/2)) * 0.1f;
    wave.x = 0;
    wave.z = 0;
    vec4 worldPos = model * vec4(aPos + wave, 1.0);
    gl_Position = projection * view * worldPos;
}