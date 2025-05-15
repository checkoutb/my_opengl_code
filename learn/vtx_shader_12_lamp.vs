#version 460

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // you dao zuo
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}