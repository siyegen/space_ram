#version 330 core
out vec4 color;

uniform vec4 outlineColor;

void main() {
    color = vec4(outlineColor);
}