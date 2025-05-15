

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>   // my_model 中用到了，但是my_model 不能写，因为会 redefine。


#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "my_shader.h"
#include "camera.h"
#include "my_model.h"


#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>


/*

export LD_LIBRARY_PATH=/mnt/239G/z_my_backup/opengl/assimp-5.4.3/bin:$LD_LIBR
ARY_PATH

-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I/mnt/239G/z_my_backup/opengl/my_linux/glad/include -lmyglad -L./ -I/mnt/239G/z_my_backup/opengl/assimp-5.4.3/include/ -L/mnt/239G/z_my_backup/opengl/assimp-5.4.3/bin/ -lassimp

。ok，可以加载，但是确实慢。需要1分钟左右吧。 都用在 processNode 上了。

。。加了cache 确实可以。 一共79个Children，之前是 load 了79次， 现在加上cache后，只需要load 2次 。。其他的都走 cache 了。。

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 300;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* wnd = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learn", NULL, NULL);
    if (wnd == NULL) {
        std::cerr<<"ERROR: glfw wnd: failed"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(wnd);
    glfwSetFramebufferSizeCallback(wnd, framebuffer_size_callback);
    glfwSetCursorPosCallback(wnd, mouse_callback);
    glfwSetScrollCallback(wnd, scroll_callback);
    
    glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr<<"ERROR: glad: load failed"<<std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    MyShader myShader("vtx_shader_20.vs", "frg_shader_20.fs");

    char mp[] = "/mnt/239G/z_my_backup/opengl/3D_guitar_bag/backpack.obj";
    
    My_Model myModel(mp);
    while (!glfwWindowShouldClose(wnd)) {
        float curr = (float) glfwGetTime();
        deltaTime = curr - lastFrame;
        lastFrame = curr;

        processInput(wnd);

        glClearColor(.05f, .05f, .05f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(myShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(myShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        

        glm::mat4 model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(.0f, .0f, .0f));
        model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

        glUniformMatrix4fv(glGetUniformLocation(myShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        myModel.draw(myShader);
        
        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}





// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
