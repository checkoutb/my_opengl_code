

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "my_shader.h"
#include "camera.h"
#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>


/*


-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./







*/



float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
    stbi_set_flip_vertically_on_load(true);

    int w, h, nr;
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);  // active.
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load("/mnt/239G/z_my_backup/opengl/container2.png", &w, &h, &nr, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr<<"ERROR: texture: failed load picture"<<std::endl;
    }

    // ----- texture 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("/mnt/239G/z_my_backup/opengl/specular_map.png", &w, &h, &nr, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr<<"ERROR: texture: failed load specular_map.png"<<std::endl;
    }
    


    // -------------- shader
    MyShader ms("vtx_shader_14.vs", "frg_shader_15.fs");

    ms.use();
    glUniform1i(glGetUniformLocation(ms.getId(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(ms.getId(), "material.specular"), 1);
    glUniform3f(glGetUniformLocation(ms.getId(), "light.dir"), -.2f, -1.f, -.3f);

    // light
    MyShader ls("vtx_shader_12_lamp.vs", "frg_shader_11_light.fs");
    

    // -------------- model, view, projection    camera
    // Camera cam(glm::vec3(0.f, 0.f, 3.f));

    // light position, scroll
    glm::vec3 lightPos(1.2f, 1.f, 2.f);

    
    ms.use();
    glUniform3f(glGetUniformLocation(ms.getId(), "lightPos"), 1.2f, 1.f, 2.f);
    glUniform3f(glGetUniformLocation(ms.getId(), "light.pos"), 1.2f, 1.f, 2.f);
    
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
    size_t row_sz = 8 * flt_sz;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, row_sz, (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, row_sz, (void*) (3 * flt_sz));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, row_sz, (void*) (6 * flt_sz));
    glEnableVertexAttribArray(2);

    
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



    glm::vec3 cubePositions[] = {
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


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        
        ms.use();
        glUniform3f(glGetUniformLocation(ms.getId(), "objCol"), 1.f, 0.5f, 0.3f);
        glUniform3f(glGetUniformLocation(ms.getId(), "lightCol"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(ms.getId(), "viewPos"), cam.Position[0], cam.Position[1], cam.Position[2]);

        // glUniform3f(glGetUniformLocation(ms.getId(), "material.ambient"), 1.f, .5f, .3f);
        // glUniform3f(glGetUniformLocation(ms.getId(), "material.diffuse"), 1.f, 0.5f, .3f);
        // glUniform3f(glGetUniformLocation(ms.getId(), "material.specular"), .5f, .5f, .5f);
        glUniform1f(glGetUniformLocation(ms.getId(), "material.shininess"), 32.0f);

        glm::vec3 lightCol;
        lightCol.x = sin(glfwGetTime() * 2.f);
        lightCol.y = sin(glfwGetTime() * .7f);
        lightCol.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffCol = lightCol * glm::vec3(0.5f);
        glm::vec3 ambCol = diffCol * glm::vec3(0.2f);

        // glUniform3f(glGetUniformLocation(ms.getId(), "light.amb"), ambCol.x, ambCol.y, ambCol.z);
        // glUniform3f(glGetUniformLocation(ms.getId(), "light.dif"), diffCol.x, diffCol.y, diffCol.z);
        glUniform3f(glGetUniformLocation(ms.getId(), "light.amb"), .1f, .1f, .1f);
        glUniform3f(glGetUniformLocation(ms.getId(), "light.dif"), .8f, .8f, .8f);
        glUniform3f(glGetUniformLocation(ms.getId(), "light.spe"), 1.f, 1.f, 1.f);
        glUniform1f(glGetUniformLocation(ms.getId(), "light.constant"), 1.f);
        glUniform1f(glGetUniformLocation(ms.getId(), "light.linear"), .09f);
        glUniform1f(glGetUniformLocation(ms.getId(), "light.quadratic"), .032f);

        
        glUniform3f(glGetUniformLocation(ms.getId(), "light.flashLightPos"), cam.Position.x, cam.Position.y, cam.Position.z);
        glUniform3f(glGetUniformLocation(ms.getId(), "light.flashLightDir"), cam.Front.x, cam.Front.y, cam.Front.z);
        glUniform1f(glGetUniformLocation(ms.getId(), "light.flashLightCutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(ms.getId(), "light.outerCutOff"), glm::cos(glm::radians(17.5f)));

        
        // model, view, projection
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.Zoom), 2.f, .1f, 100.f);

        // glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        
        glBindVertexArray(vao);

        for (size_t i = 0; i < 10; ++i) {
            model = glm::identity<glm::mat4>();
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, .5f));
            glUniformMatrix4fv(glGetUniformLocation(ms.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

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


