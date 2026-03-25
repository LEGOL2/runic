#version 460 core

uniform mat4 uModel;
uniform mat4 uProjection;

layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = uProjection * uModel *  vec4(aPos, 1.0);
}
