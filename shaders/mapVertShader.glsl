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
uniform float drawDistance;

out vec3 pos;

void main() {
    pos = aPos;
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;
}