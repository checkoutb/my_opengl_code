#include <iostream>

////  /* -*- eval: (setq-local eglot-workspace-configuration '(:clangd (:args ("-I/mnt/239G/z_my_backup/opengl/my_linux/glad/include")))) -*- */

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

`./a.out`   出现一个显示框，名字是 LearnOpenGL，框中是全黑的。   不过我是 sway，所以不知道 框的 原生大小
。。callback中添加输入后， ./a.out 后立刻会 输出一次， 应该就是 swap把框体 改变了导致的。。不，标准代码中 main中没有 glViewport，所以 不是 sway改变了窗口大小， 而是 第一次画的时候 就是这样的。 就是 改变 说明 从一个状态 到 另一个状态，所以 一共有 2个状态， 但是 估计 实际上 只有一个状态，一次性就画出来了。 不是 画完 再 修改 窗口大小。   。。不过不清楚 我这种  main里 带 glViewport的 写法，是 2个状态 还是 一个状态，应该是2个吧？



能显示三角形，不过代码和 官方代码 差异还是有点的
https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp


2个三角形组成的矩形， 和标准代码对比后才 能显示出来。
原先，我注释了 VBO，并且 glDrawElements 在 循环外。。

随时间渐变可以，但调色盘做不出来。。

g，准备直接抄标准代码。

这个是根据标准代码改的，可以。

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
    

    // glViewport(0, 0, 800, 600);    // 标准代码中，main方法中没有这行； callback 中有 glViewport

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // // 顶点属性个数的上限
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;   // 16


    

    const char* vertexShaderSource =
        "#version 460 core\n"      // 330, 460
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aCol;\n"
        //"out vec4 vtxCol;"  // 给片段着色器用。  同名变量
        "out vec3 myCol;\n"
        "void main()\n"
        "{\n"
        // "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        // "   vtxCol = vec4(0.5, 0.0, 0.0, 1.0);"   // 之前忘了这句，窗口显示纯色。
        "gl_Position = vec4(aPos, 1.0);\n"
        "myCol = aCol;\n"
        "}\0";

    

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    // 检查是否成功
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // 片段着色器
    // const char* fragmentShaderSource =
    //     "#version 460 core\n"      // 330, 460
    //     "out vec4 FragColor;\n"
    //     "void main()\n"
    //     "{\n"
    //     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    //     "}\0";


    // const char* fragmentShaderSource =
    //     "#version 460 core\n"      // 330, 460
    //     "out vec4 FragColor;\n"
    //     "in vec4 vtxCol;"
    //     "void main()\n"
    //     "{\n"
    //     "   FragColor = vtxCol;\n"   // 直接使用 顶点着色器的 vtxCol
    //     "}\0";


    const char* fragmentShaderSource =
        "#version 460 core\n"      // 330, 460
        "out vec4 FragColor;\n"
        // "in vec4 vtxCol;"
        // "uniform vec4 myCol;"
        "in vec3 myCol;\n"   // 使用 vectrics 中的颜色，而不是 uniform的
        "void main()\n"
        "{\n"
        "   FragColor = vec4(myCol, 1.0f);\n"   // 直接使用 顶点着色器的 vtxCol
        "}\0";

    
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"error... "<<infoLog<<std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    

    unsigned int VAO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);



    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glGenVertexArrays(1, &VAO);
    // ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
    // 1. 绑定VAO


    

    // 试下来，下面的语句的相对顺序不能变化。
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);    // 这个1应该就是 上面的第一个参数，应该也是 GLSL中 layout(location = ) 的值



    
    
    // 2. 把顶点数组复制到缓冲中供OpenGL使用


    // 3. 设置顶点属性指针
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // ..:: 绘制代码（渲染循环中） :: ..
    // 4. 绘制物体



    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glUseProgram(shaderProgram);

    // 线
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // GL_FILL 是默认的
    

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // glUseProgram(shaderProgram);
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
