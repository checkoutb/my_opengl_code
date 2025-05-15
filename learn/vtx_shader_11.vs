#version 460

// layout (location = 0) in vec3 aPos;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 proj;

// void main() {
//     // 右到左 计算顺序
//     gl_Position = proj * view * model * vec4(aPos, 1.0);
// }



// 12 basic lighting

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 frg_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    // Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;   // 高消耗的操作
    frg_pos = vec3(model * vec4(aPos, 1.0));
}



