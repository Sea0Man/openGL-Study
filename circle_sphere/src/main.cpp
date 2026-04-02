
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "camera.h"
#include <iostream>
#include <math.h>

#define HEIGTH 900
#define WIDTH 1600
#define SECTIONS 16

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float angle = 2 * (float)M_PI / SECTIONS;

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
    unsigned int indices[SECTIONS][SECTIONS * 6];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    Shader shader("../shaders/vertexShader.txt", "../shaders/fragmentShader.txt");

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    
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
        for (int j = 0; j < SECTIONS - 1; j++)
        {
            indices[i][6 * j    ] = i * SECTIONS + j;
            indices[i][6 * j + 1] = i * SECTIONS + ((j + 1) % SECTIONS);
            indices[i][6 * j + 2] = ((i + 1) % SECTIONS) * SECTIONS + j;

            indices[i][6 * j + 3] = ((i + 1) % SECTIONS) * SECTIONS + ((j + 1) % SECTIONS);
            indices[i][6 * j + 4] = i * SECTIONS + ((j + 1) % SECTIONS);
            indices[i][6 * j + 5] = ((i + 1) % SECTIONS) * SECTIONS + j;
        }
    }
    

    

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();
    

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / 2, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", model);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_POINTS, 0, sizeof(sphere));

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}
