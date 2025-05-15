#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec3 frg_pos;
out vec2 myTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    
    // Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;   // 高消耗的操作
    frg_pos = vec3(model * vec4(aPos, 1.0));
    myTex = aTex;
    gl_Position = proj * view * vec4(frg_pos, 1.0f);
}





// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;

// out vec3 FragPos;
// out vec3 Normal;
// out vec2 TexCoords;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// void main()
// {
//     FragPos = vec3(model * vec4(aPos, 1.0));
//     Normal = mat3(transpose(inverse(model))) * aNormal;  
//     TexCoords = aTexCoords;
    
//     gl_Position = projection * view * vec4(FragPos, 1.0);
// }

