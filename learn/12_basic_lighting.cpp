

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "my_shader.h"
#include "camera.h"
#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


/*


-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./


1. 环境光照可以， 变黑了
2. 漫反射不行，还是黑的。。 ok了。  lightPos 是ms的， 而且不能使用 glUniformMatrix 来设置值。

3. 视角的移动还是有点缺陷，好像没有办法看上面。 就是 摄像机 只能左右平移， 不能上下 平移，    目前 上下 是 摄像机的 靠近 远离  。当然 视角 拉远，是可以 有办法看到 上面 和下面的。
    懂了， yh 只能前进和后退，但是 鼠标可以拉视角， 所以 鼠标使得视角45度朝上，后退的话，摄像机就会 往下走。





*/



float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



int indices[] = {};

Camera cam(glm::vec3(0.f, 0.f, 3.f));
float deltaTime = 0.f;
float lastFrame = .0f;


void framebuffer_callback(GLFWwindow* wnd, int w, int h) {
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow* wnd) {
    if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, true);
    }
    // 不能 else if：如果按住2个键 往左下。
    if (glfwGetKey(wnd, GLFW_KEY_Y) == GLFW_PRESS) {
        cam.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(wnd, GLFW_KEY_H) == GLFW_PRESS) {
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(wnd, GLFW_KEY_T) == GLFW_PRESS) {
        cam.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(wnd, GLFW_KEY_U) == GLFW_PRESS) {
        cam.ProcessKeyboard(RIGHT, deltaTime);
    }
}

bool fst_mus = true;
float lastx, lasty;
void mouse_callback(GLFWwindow* wnd, double x, double y) {
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);

    if (fst_mus) {
        lastx = xpos;
        lasty = ypos;
        fst_mus = false;
    }

    float xoff = xpos - lastx;
    float yoff = lasty - ypos;

    lastx = xpos;
    lasty = ypos;

    cam.ProcessMouseMovement(xoff, yoff);
}

void scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset) {
    cam.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* wnd = glfwCreateWindow(800, 600, "opengl-learn", NULL, NULL);
    if (wnd == NULL) {
        std::cerr<<"ERROR: glfwwindow: create failed."<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(wnd);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr<<"ERROR: glad: failed to load gl loader."<<std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(wnd, framebuffer_callback);
    glfwSetCursorPosCallback(wnd, mouse_callback);
    glfwSetScrollCallback(wnd, scroll_callback);

    glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    
    size_t flt_sz = sizeof(float);

    glfwSwapInterval(5);
    
    // -------------- texture


    // -------------- shader
    MyShader ms("vtx_shader_11.vs", "frg_shader_11.fs");
    ms.use();

    // light
    MyShader ls("vtx_shader_12_lamp.vs", "frg_shader_11_light.fs");
    ls.use();
    

    // -------------- model, view, projection    camera
    // Camera cam(glm::vec3(0.f, 0.f, 3.f));

    // light position, scroll
    glm::vec3 lightPos(1.2f, 1.f, 2.f);

    
    ms.use();
    glUniform3f(glGetUniformLocation(ms.getId(), "lightPos"), 1.2f, 1.f, 2.f);

    
    // --------------VAO, VBO, EBO
    unsigned int vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glGenBuffers(1, &ebo);
    // glBindBuffer(GL_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // decode vertex data
    size_t row_sz = 6 * flt_sz;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, row_sz, (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, row_sz, (void*) (3 * flt_sz));
    glEnableVertexAttribArray(1);

    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, row_sz, (void*) (3 * flt_sz));
    // glEnableVertexAttribArray(1);


    // ------------- light
    unsigned int lightvao;
    glGenVertexArrays(1, &lightvao);
    glBindVertexArray(lightvao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // decode
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, row_sz, (void*) 0);
    glEnableVertexAttribArray(0);    // 看标准代码，这里也可以是0，估计index是绑定在 buffer上。



    // -------------- 

    while (!glfwWindowShouldClose(wnd)) {

        float curr = static_cast<float>(glfwGetTime());
        deltaTime = curr - lastFrame;
        lastFrame = curr;
        
        // ----
        process_input(wnd);

        // ---- render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ms.use();
        glUniform3f(glGetUniformLocation(ms.getId(), "objCol"), 1.f, 0.5f, 0.3f);
        glUniform3f(glGetUniformLocation(ms.getId(), "lightCol"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(ms.getId(), "viewPos"), cam.Position[0], cam.Position[1], cam.Position[2]) ;


        // model, view, projection
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.Zoom), 2.f, .1f, 100.f);

        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // lamp
        model = glm::identity<glm::mat4>();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        ls.use();
        glUniformMatrix4fv(glGetUniformLocation(ls.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ls.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ls.getId(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(lightvao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &lightvao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}


