#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

enum camera_direction{
    FRONT,
    BACK,
    RIGHT,
    LEFT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;
const float SENSITIVITY = 0.1f;
const float SPEED = 0.01f;


class Camera{
public:
    glm::vec3 CameraPos;
    glm::vec3 CameraFront;
    glm::vec3 CameraUp;
    glm::vec3 GlobalUp;
    glm::vec3 CameraRight;

    float Yaw;
    float Pitch;
    float Fov;

    float MouseSensitivity;
    float Speed;

    Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 globalUp = glm::vec3(0.0f, 1.0f,  0.0f), float yaw = YAW, float pitch = PITCH) : CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), Fov(FOV), MouseSensitivity(SENSITIVITY), Speed(SPEED){
        CameraPos = cameraPos;
        GlobalUp = globalUp;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVector();
    } 

    glm::mat4 getViewMatrix(){
        return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
    }

    void processKeyboard(camera_direction direction, float deltaTime){

        float velocity = Speed * deltaTime;
        if(direction == FRONT)
            CameraPos += glm::vec3(CameraFront.x, 0.0f, CameraFront.z) * velocity;
        if(direction == BACK)
            CameraPos -= glm::vec3(CameraFront.x, 0.0f, CameraFront.z) * velocity;
        if(direction == RIGHT)
            CameraPos -= glm::vec3(CameraRight.x, 0.0f, CameraRight.z) * velocity;
        if(direction == LEFT)
            CameraPos += glm::vec3(CameraRight.x, 0.0f, CameraRight.z) * velocity;
    }

    void processMouse(float Xoffset, float Yoffset)
    {
        Yaw -= Xoffset * MouseSensitivity;
        Pitch -= Yoffset * MouseSensitivity;

        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVector();
    }

    void processScroll(float Yoffset){
        Fov -= (float)Yoffset;
        if(Fov > 45.0f)
            Fov = 45.0f;
        if(Fov < 1.0f)
            Fov = 1.0f;
    }

private:
    void updateCameraVector()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        CameraFront = glm::normalize(direction);

        CameraRight = glm::normalize(glm::cross(GlobalUp, CameraFront));
        CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
    }
};