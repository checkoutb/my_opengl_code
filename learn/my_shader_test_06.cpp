#include <iostream>

////  /* -*- eval: (setq-local eglot-workspace-configuration '(:clangd (:args ("-I/mnt/239G/z_my_backup/opengl/my_linux/glad/include")))) -*- */

#include "my_shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>


// 为了让 emacs/LSP(clangd) 能找到 glag的 头文件。试了好几种，只有 .clang 是可行的。

/*

到 glad的 src文件加中， 编译 并 打包成静态库
`g++ glad.c -c -I../include`
`ar rcs libmyglad.a glad.o`

把 .a 复制到 my_opengl_code/learn 下

`g++ 01_test_run.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`

`./a.out`

 */


// float vertices[] = {
//     -0.5f, -0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f,  0.5f, 0.0f
// };


float vertices[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
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

    MyShader ms("./vtx_shader.vs", "./frg_shader.fs");
    ms.use();

    unsigned int VAO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);

    // 试下来，下面的语句的相对顺序不能变化。
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);    // 这个1应该就是 上面的第一个参数，应该也是 GLSL中 layout(location = ) 的值

    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 线
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // GL_FILL 是默认的
    

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}


/*--*/ // Local Variables:
// eglot-workspace-configuration: (:clangd (:args ("-I/mnt/239G/z_my_backup/opengl/my_linux/glad/include")))
// End:

int fun() {
    return 0;
}
