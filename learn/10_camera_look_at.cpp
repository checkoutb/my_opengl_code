
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include "my_shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>   // copy into xxx/glad/include/


#include <glm/glm.hpp>   // copy -> xxx/glad/include/
#include <glm/gtc/matrix_transform.hpp>  // glm::mat4 mat = glm::mat4(1.0f);  glm::mat4 trans = glm::identity<glm::mat4>();
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <unistd.h>

/*

到 glad的 src文件加中， 编译 并 打包成静态库
`g++ glad.c -c -I../include`
`ar rcs libmyglad.a glad.o`

把 .a 复制到 my_opengl_code/learn 下

`g++ 01_test_run.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./`

`./a.out`



tyu
 h


鼠标移动视角，非常不协调 不知道是不是 万向节死锁 导致的。   不清楚 万向节死锁 的 表现是什么。
似乎是的， 但是 我每次运行 都会出现 万向节死锁。。不会这么频繁吧。

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

// float vertices[] = {
// //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
// };



float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



unsigned int indices[] = {
    0,1,3,
    1,2,3
};

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);


float deltaTime = 0.0f;
float lastFrame = 0.0f;



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


    float camSpd = 2.5f * deltaTime;
    if (glfwGetKey(wnd, GLFW_KEY_Y) == GLFW_PRESS) {
        camPos += camSpd * camFront;
    } else if (glfwGetKey(wnd, GLFW_KEY_H) == GLFW_PRESS) {
        camPos -= camSpd * camFront;
    } else if (glfwGetKey(wnd, GLFW_KEY_T) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpd;
    else if (glfwGetKey(wnd, GLFW_KEY_U) == GLFW_PRESS) {
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpd;
    }
}


float lastx = 400, lasty = 300;
float yaw = -90.0f, pitch = -10010.f;
bool fst = true;
void mouse_callback(GLFWwindow* wnd, double xpos, double ypos) {

    // if (pitch + 9999.f < 0.f) {
    if (fst) {
        fst = false;
        pitch = 0.f;
        lastx = xpos;
        lasty = ypos;
    }
    
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;   // ------ last - pos
    lastx = xpos;
    lasty = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch >= 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.f) {
        pitch = -89.f;
    }

    glm::vec3 fnt;
    fnt.x = cos(glm::radians(yaw)) * sin(glm::radians(pitch));
    fnt.y = sin(glm::radians(pitch));
    fnt.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(fnt);
}

float fov = 45.0f;
void scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset) {
    // if (fov >= 1.0f && fov <= 45.0f) {
        fov -= yoffset;
    // }
    if (fov <= 1.0f) {
        fov = 1.0f;
    } else if (fov >= 45.0f) {
        fov = 45.0f;
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


    // --- hide and catch mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetScrollCallback(window, scroll_callback);
    


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
    // glm::mat4 trans = glm::mat4(1.0f);
    // // glm::mat4 trans = glm::identity<glm::mat4>();
    
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, -0.5, 0.5));

    // unsigned int transLoc = glGetUniformLocation(ms.getId(), "myTrans");
    // glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));


    glm::mat4 model = glm::identity<glm::mat4>();
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::identity<glm::mat4>();
    view = glm::translate(view, glm::vec3(0.0f, 0.f, -2.0f));

    glm::mat4 proj = glm::identity<glm::mat4>();
    proj = glm::perspective(glm::radians(45.0f), 600.0f / 300.0f, 0.1f, 100.f);
    

    glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));


    glEnable(GL_DEPTH_TEST);


    
    

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

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glfwSwapInterval(5);

    glm::vec3 cubePos[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    
    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // image rotate
        // glm::mat4 trans = glm::mat4(1.0f);

        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.f, 1.f));        
        // glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);


        // rotate
        // glm::mat4 view = glm::identity<glm::mat4>();
        // float radius = 10.f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(.0, 1.0, 0.0));
        // glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // self move
        glm::mat4 view = glm::identity<glm::mat4>();
        

        view = glm::lookAt(camPos, camPos + camFront, camUp);
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        



        glm::mat4 proj = glm::identity<glm::mat4>();
        proj = glm::perspective(glm::radians((float) fov), 600.0f / 300.0f, 0.1f, 100.f);
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));




        
        for (int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::identity<glm::mat4>();
            model = glm::translate(model, cubePos[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        float currFm = glfwGetTime();
        deltaTime = currFm - lastFrame;
        lastFrame = currFm;        

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
