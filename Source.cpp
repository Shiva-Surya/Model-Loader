#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include"Mesh.h"
#include"Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ObjectColor = glm::vec3(0.3f, 0.4f, 0.5f);


float del = 0.0f;
float last = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX;
float lastY;
float fov = 45.0f;

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Model Loader :: Press Escape to Exit", monitor, NULL);

    lastX = mode->width / 2.0;
    lastY = mode->height / 2.0;
    float aspect = lastX / lastY;

    /*GLFWwindow* window;
    window = glfwCreateWindow(mode->width, mode->height, "Model Loader :: Press Escape to Exit", NULL, NULL);*/
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glViewport(0, 0, mode->width, mode->height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);



    glEnable(GL_DEPTH_TEST);

    Shader ModelProgram("shaders/LightVertex.ver", "shaders/LightFragment.frag");
    Model TestModel;
    TestModel.loadModel("models/tyre/scene.gltf");
    


    while (!glfwWindowShouldClose(window))
    {
        float cur = glfwGetTime();
        del = cur - last;
        last = cur;

        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        ModelProgram.use();
        ModelProgram.setVec3("light.ambient",glm::vec3(0.05f) );
        ModelProgram.setVec3("light.diffuse", glm::vec3(0.7f));
        ModelProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        ModelProgram.setVec3("light.position", CamPos);
        ModelProgram.setVec3("light.direction", CamFront);
        ModelProgram.setFloat("light.A", 1.0f);
        ModelProgram.setFloat("light.B", 0.09f);
        ModelProgram.setFloat("light.C", 0.032f);
        ModelProgram.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        ModelProgram.setFloat("light.OcutOff", glm::cos(glm::radians(12.5f)));
        
        ModelProgram.setVec3("viewPos", CamPos);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(CamPos, CamFront + CamPos, CamUp);
        glm::mat4 model = glm::mat4(1.0f);

        ModelProgram.setMat4("projection", projection);
        ModelProgram.setMat4("view", view);
        ModelProgram.setMat4("model", model);

        TestModel.Draw(ModelProgram);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{

    glm::vec3 Up;
    Up.x = cos(glm::radians(yaw)) * sin(glm::radians(pitch));
    Up.y = cos(glm::radians(pitch));
    Up.z = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
    CamUp = glm::normalize(Up);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float speed = 2.0f * del;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CamPos += CamFront * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CamPos -= CamFront * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CamPos += glm::normalize(glm::cross(CamFront, CamUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CamPos -= glm::normalize(glm::cross(CamFront, CamUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        CamPos = glm::vec3(0.0f, 0.0f, 5.0f); 
        CamFront = glm::vec3(0.0f, 0.0f, -1.0f); 
        CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }    
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    CamFront = glm::normalize(front);
    glm::vec3 Up;
    Up.x = cos(glm::radians(yaw)) * sin(glm::radians(pitch));
    Up.y = cos(glm::radians(pitch));
    Up.z = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
    CamUp = glm::normalize(Up);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    std::cout << xoffset << yoffset << std::endl;
    if (fov < 1.0f)
        fov = 1.0f;
    /*if (fov > 90.0f)
        fov = 90.0f;*/
}