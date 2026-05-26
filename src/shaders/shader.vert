#version 450

layout(location = 0) in vec2 vertPos;
layout(location = 2) in vec3 vertCol;

layout(location = 0) out vec3 vertColor;

void main() {
    gl_Position = vec4(vertPos, 0.0, 1.0);
    vertColor = vertCol;
}