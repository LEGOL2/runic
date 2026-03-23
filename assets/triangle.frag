#version 460 core

uniform float uTime;

out vec4 FragColor;

void main() {
    float r = sin(uTime * 1.0) * 0.5 + 0.5;
    float g = sin(uTime * 1.5 + 2.094) * 0.5 + 0.5;  // 2.094 = 2π/3
    float b = sin(uTime * 2.0 + 4.189) * 0.5 + 0.5;  // 4.189 = 4π/3
    FragColor = vec4(r, g, b, 1.0);
}
