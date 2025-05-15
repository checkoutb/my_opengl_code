
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "my_shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>   // copy into xxx/glad/include/


#include <glm/glm.hpp>   // copy -> xxx/glad/include/
#include <glm/gtc/matrix_transform.hpp>  // glm::mat4 mat = glm::mat4(1.0f);
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>

/*

到 glad的 src文件加中， 编译 并 打包成静态库
`g++ glad.c -c -I../include`
`ar rcs libmyglad.a glad.o`

把 .a 复制到 my_opengl_code/learn 下

`g++ 01_test_run.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`

`./a.out`


1. 标准代码中 vertics 是 5列。 所以 fragment shader 需要注意。
2. 我怎么是顺时针转了90度， 教程的图是 逆时针  .没有解决  。 可以运行时一直旋转。 
     解决了，估计是教程的问题， 教程中是 0.5， 但是代码中 是 -0.5。 而且 代码是 一直旋转的代码， 没有提供 旋转90度的代码，估计就是 使用 一直旋转的 代码 注释掉 while 中的 旋转代码， 来截图的。


*/


// float vertices[] = {
//     -0.5f, -0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f,  0.5f, 0.0f
// };


// float vertices[] = {
//     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
//     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
//     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
// };

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // std::cout<<"oooo"<<std::endl;
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* wnd) {
    if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout<<"got esc, exit..."<<std::endl;
        glfwSetWindowShouldClose(wnd, true);
    }

    if (glfwGetKey(wnd, GLFW_KEY_3) == GLFW_PRESS) {
        std::cout<<"got 33\n";   // 输出太多了，每次渲染都会输出一次。
    }
}

int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // ---------------------- texture
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;

    unsigned int texture;
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);  //
    
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load("/mnt/239G/z_my_backup/opengl/texture.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cout<<"ERROR: texture: failed load picture"<<std::endl;
    }

    // ----

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    data = stbi_load("/mnt/239G/z_my_backup/opengl/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);   // png => RGBA
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cout<<"ERROR: texture: failed load face.png"<<std::endl;
    }

    
    
    MyShader ms("./vtx_shader.vs", "./frg_shader.fs");
    ms.use();

    ms.setInt("myTexture1", 0);
    ms.setInt("myTexture2", 1);



    // --------------matrix
    glm::mat4 trans = glm::mat4(1.0f);
    // glm::mat4 trans = glm::identity<glm::mat4>();
    
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, -0.5, 0.5));

    unsigned int transLoc = glGetUniformLocation(ms.getId(), "myTrans");
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));



    

    unsigned int VAO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // image rotate
        glm::mat4 trans = glm::mat4(1.0f);

        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.f, 1.f));        
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
