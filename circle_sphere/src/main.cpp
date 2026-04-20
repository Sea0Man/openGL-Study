
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "camera.h"
#include <iostream>
#include <math.h>

#define HEIGTH 675
#define WIDTH 1200
#define SECTIONS 128

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float angle = 2 * (float)M_PI / SECTIONS;
glm::vec3 lightDirection = glm::vec3(1.0f, -1.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 1.0f);

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
int main(int, char**){


    // float circle[SECTIONS * 3];
    float sphere[SECTIONS][SECTIONS * 3];
    unsigned int sphere_Indices[SECTIONS][SECTIONS * 6];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGTH, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    glEnable(GL_DEPTH_TEST);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader("../shaders/object.vs", "../shaders/object.fs");
    
    

    
    // for (int i = 0; i < SECTIONS; i++)
    // {
    //     circle[i * 3] = cos(anlge * i);
    //     circle[i * 3 + 1] = sin(anlge * i);
    //     circle[i * 3 + 2] = 0;
    // }

    float sinAngle, cosAngle, x, z, scale;
    for (int i = 0; i < SECTIONS; i++)
    {
        for (int j = 0; j < SECTIONS; j++)
        {
            sphere[i][3 * j] = cos(angle * i) * cos(angle * j);
            sphere[i][3 * j + 1] = sin(angle * i) * cos(angle * j);
            sphere[i][3 * j + 2] = sin(angle * j);

        }
    }

    for (int i = 0; i < SECTIONS; i++)
    {
        for (int j = 0; j < SECTIONS; j++)
        {
            sphere_Indices[i][6 * j    ] = i * SECTIONS + j;
            sphere_Indices[i][6 * j + 1] = i * SECTIONS + ((j + 1) % SECTIONS);
            sphere_Indices[i][6 * j + 2] = ((i + 1) % SECTIONS) * SECTIONS + j;

            sphere_Indices[i][6 * j + 3] = ((i + 1) % SECTIONS) * SECTIONS + ((j + 1) % SECTIONS);
            sphere_Indices[i][6 * j + 4] = i * SECTIONS + ((j + 1) % SECTIONS);
            sphere_Indices[i][6 * j + 5] = ((i + 1) % SECTIONS) * SECTIONS + j;
        }
    }


    

    unsigned int sphere_VAO, sphere_VBO, sphere_EBO;


    glGenBuffers(1, &sphere_VBO);
    glGenBuffers(1, &sphere_EBO);
    glGenVertexArrays(1, &sphere_VAO);

    glBindVertexArray(sphere_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_Indices), sphere_Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 
    
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        shader.use();
        shader.setVec3("light.direction", lightDirection);
        shader.setVec3("viewPos", camera.CameraPos);
        shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.specular", glm::vec3(0.1f));
        shader.setFloat("material.shininess", 6.0f);
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());


        glBindVertexArray(sphere_VAO);
        glDrawElements(GL_TRIANGLES, sizeof(sphere_Indices) / sizeof(float), GL_UNSIGNED_INT, 0);

        

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}
