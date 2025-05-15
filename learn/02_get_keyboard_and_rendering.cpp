#include <iostream>

////  /* -*- eval: (setq-local eglot-workspace-configuration '(:clangd (:args ("-I/mnt/239G/z_my_backup/opengl/my_linux/glad/include")))) -*- */

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// 为了让 emacs/LSP(clangd) 能找到 glag的 头文件。试了好几种，只有 .clang 是可行的。

/*

到 glad的 src文件加中， 编译 并 打包成静态库
`g++ glad.c -c -I../include`
`ar rcs libmyglad.a glad.o`

把 .a 复制到 my_opengl_code/learn 下

`g++ 01_test_run.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`

`./a.out`   出现一个显示框，名字是 LearnOpenGL，框中是全黑的。   不过我是 sway，所以不知道 框的 原生大小
。。callback中添加输入后， ./a.out 后立刻会 输出一次， 应该就是 swap把框体 改变了导致的。






*/


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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 。使用 core 模式
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 。真正起效  。。不知道什么意思了，好像加不加 没区别。 ... mac os 才需要加， 其他的加不加都起效。

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
    

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
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
