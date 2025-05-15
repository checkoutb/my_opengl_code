#version 460 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aCol;
// out vec3 myCol;
// void main()
// {
//     gl_Position = vec4(aPos, 1.0);
//     myCol = aCol;
// }


// 07_texture
// #version 460 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aCol;
// layout (location = 2) in vec2 aTex;

// out vec3 myCol;
// out vec2 myTex;

// void main()
// {
//     gl_Position = vec4(aPos, 1.0);
//     myCol = aCol;
//     myTex = aTex;
// }


// 08 matrix
// layout (location = 0) in vec3 aPos;
// layout (location = 2) in vec2 aTex;

// out vec2 myTex;

// uniform mat4 myTrans;

// void main() {
//     gl_Position = myTrans * vec4(aPos, 1.0f);
//     myTex = vec2(aTex.x, 1.0 - aTex.y); 
// }


// 09 coordinate 3D
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 myTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // you dao zuo
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    myTex = vec2(aTex.x, aTex.y);   // 1-aTex.y 图像颠倒
}





