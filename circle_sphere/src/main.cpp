
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

Camera camera(glm::vec3(0.0f, -1.0f, 7.0f));
float angle = 2 * (float)M_PI / SECTIONS;
glm::vec3 lightPos = glm::vec3(3.0f, -0.5f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 1.0f);

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

    Shader shader("../shaders/object.vs", "../shaders/object.fs");
    Shader floor_shader("../shaders/floor.vs", "../shaders/floor.fs");
    Shader light_shader("../shaders/light.vs", "../shaders/light.fs");

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

    float floor_vertices[] = {
         1.0f,  0.0f, 1.0f, 
         1.0f,  0.0f, -1.0f, 
        -1.0f,  0.0f, 1.0f, 
        -1.0f,  0.0f, -1.0f 
    };
    unsigned int floor_indices[] = {
        0, 1, 2,
        3, 1, 2
    };

    float light_cube_vertices[] = {
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
    };

    unsigned int light_cube_indices[] = {
        0, 1, 2,
        3, 1, 2,
        4, 5, 6,
        7, 5, 6,

        0, 1, 4,
        5, 1, 4,
        2, 3, 6,
        7, 3, 6,

        0, 2, 4,
        6, 2, 4,
        1, 3, 5,
        7, 3, 5
    };

    

    

    unsigned int light_cube_VAO, sphere_VAO, light_cube_VBO, light_cube_EBO, sphere_VBO, sphere_EBO, floor_VAO, floor_VBO, floor_EBO;


    glGenBuffers(1, &floor_VBO);
    glGenBuffers(1, &floor_EBO);
    glGenVertexArrays(1, &floor_VAO);

    glBindVertexArray(floor_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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


    glGenBuffers(1, &light_cube_VBO);
    glGenBuffers(1, &light_cube_EBO);
    glGenVertexArrays(1, &light_cube_VAO);

    glBindVertexArray(light_cube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, light_cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cube_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(light_cube_indices), light_cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    camera.processMouse(0.0f, -190.0f);
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        shader.use();
        shader.setVec3("objectColor", glm::vec3(0.3f, 0.6f, 0.7f));
        shader.setVec3("light.position", lightPos);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("viewPos", camera.CameraPos);
        shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("material.shininess", 32.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / 2, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", model);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());

        glBindVertexArray(sphere_VAO);
        glDrawElements(GL_TRIANGLES, sizeof(sphere_Indices) / sizeof(float), GL_UNSIGNED_INT, 0);

        floor_shader.use();
        floor_shader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.2f));
        floor_shader.setVec3("lightPos", lightPos);
        floor_shader.setVec3("lightColor", lightColor);
        floor_shader.setVec3("viewPos", camera.CameraPos);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(100.0f));
        floor_shader.setMat4("model", model);
        projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        floor_shader.setMat4("projection", projection);
        floor_shader.setMat4("view", camera.getViewMatrix());

        glBindVertexArray(floor_VAO);
        glDrawElements(GL_TRIANGLES, sizeof(floor_indices) / sizeof(float), GL_UNSIGNED_INT, 0);

        light_shader.use();
        light_shader.setVec3("lightColor", lightColor);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3));
        light_shader.setMat4("model", model);
        projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("view", camera.getViewMatrix());

        lightPos.x = 5.0 - glfwGetTime() * 2;

        glBindVertexArray(light_cube_VAO);
        glDrawElements(GL_TRIANGLES, sizeof(light_cube_indices) / sizeof(float), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}
