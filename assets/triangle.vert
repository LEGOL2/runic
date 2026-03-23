#version 460 core

uniform mat4 uModel;

layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = uModel *  vec4(aPos, 1.0);
}
