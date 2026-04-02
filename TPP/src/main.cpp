
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"
#include "camera.h"
#include "player.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define HEIGTH 720
#define WIDTH 1280

float deltaTime = 0.0f, lastX = WIDTH/2, lastY = HEIGTH/2, lastFrame = 0.0f;;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Player player(glm::vec3(0.0f, 0.0f, 0.0f));


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
    glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
    glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
    glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player.processKeyboard(FRONT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player.processKeyboard(BACK, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player.processKeyboard(RIGHT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player.processKeyboard(LEFT, deltaTime);

            player.updateDirection(camera.CameraFront);
            camera.updatePos(player.Position);
        }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.processScroll(ypos);
}

int main(int, char**){
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
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetScrollCallback(window, scroll_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader("../shaders/vertexShader.txt", "../shaders/fragmentShader.txt");

    float vertices[] = {
            //top
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            //bottom
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        2, 1, 3,

        4, 5, 7,
        6, 5, 7,

        1, 2, 6,
        1, 6, 5,

        0, 3, 7,
        0, 4, 7,
        
        2, 3, 6,
        3, 6, 7,

        0, 1, 5,
        0, 4, 5 
    };
    glm::vec3 groundPositions[] = {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f)
    };

    unsigned VAO, EBO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);



    int width, height, nrChannels;
    unsigned int texture_wall, texture_player;
    glGenTextures(1, &texture_wall);
    glBindTexture(GL_TEXTURE_2D, texture_wall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *data = stbi_load("../img/grass.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    shader.setInt("Texture", 0);
    
    shader.use();
    

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processInput(window);
        player.frameUpdate(deltaTime);
        camera.frameUpdate(deltaTime);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        
        for (int i = 0; i < 9; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, -groundPositions[i]);
            shader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        



        shader.setMat4("model", player.Model);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)WIDTH / (float)HEIGTH, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    
    glfwTerminate();
    return 0;
}
