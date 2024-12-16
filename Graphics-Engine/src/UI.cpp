#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "Engine.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(2880, 1680, "ModuleEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 2880, 1680);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // graphics data
    std::string modelpath = "res\\models\\blenderExports\\Ship\\Ship.obj";
    // shader 
    std::string Body_vs = "res\\shaders\\Body.vshader";
    std::string Body_fs = "res\\shaders\\Body.fshader";
    std::string Engine1_vs = "res\\shaders\\Engine1.vshader";
    std::string Engine1_fs = "res\\shaders\\Engine1.fshader";
    std::string Engine2_vs = "res\\shaders\\Engine2.vshader";
    std::string Engine2_fs = "res\\shaders\\Engine2.fshader";
    std::string Engine3_vs = "res\\shaders\\Engine3.vshader";
    std::string Engine3_fs = "res\\shaders\\Engine3.fshader";

    std::vector<std::vector<std::string>> shaders = {
        std::vector{Body_vs, Body_fs}, 
        std::vector{Engine1_vs, Engine1_fs}, 
        std::vector{Engine2_vs, Engine2_fs}, 
        std::vector{Engine3_vs, Engine3_fs}
    };
    

    Engine engine(modelpath, Body_vs, Body_fs, window);
    glEnable(GL_DEPTH_TEST);
    
    
    

    // draw 
    glm::mat4 mat = glm::mat4(1.0f);
    glm::mat4 mat2 = glm::translate(mat, glm::vec3(1.0f, 0.0f, 0.0f));
    int polyMode = 1;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        engine.GetCommand();
        engine.Move();
        engine.Simulate();
       

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { polyMode = 1 - polyMode; }
        
        if (polyMode == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
        else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}