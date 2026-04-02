#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>



const float YAW = 0.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;
const float SENSITIVITY = 0.1f;
const float DISTANCE = 4.0f;
const float SCROLLSENS = 0.2f;

class Camera{
public:
    glm::vec3 CameraPos;
    glm::vec3 Target;
    glm::vec3 CameraUp;
    glm::vec3 GlobalUp;
    glm::vec3 CameraRight;
    glm::vec3 CameraFront;

    float Yaw;
    float Pitch;
    float Fov;
    float Distance;

    float MouseSensitivity;
    float Speed;

    Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 globalUp = glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Fov(FOV), Distance(DISTANCE), MouseSensitivity(SENSITIVITY){
        CameraPos = cameraPos;
        GlobalUp = globalUp;
        Yaw = yaw;
        Pitch = pitch;
        Target = target;
        updateCameraVector();
    } 

    glm::mat4 getViewMatrix(){
        return glm::lookAt(CameraPos, Target, CameraUp);
    }

    

    void processMouse(float Xoffset, float Yoffset)
    {
        Yaw -= Xoffset * MouseSensitivity;
        Pitch -= Yoffset * MouseSensitivity;

        if(Pitch > 79.0f)
            Pitch = 79.0f;
        if(Pitch < -79.0f)
            Pitch = -79.0f;

    }

    void updatePos(glm::vec3 target)
    {
        Target = target;
    }

    void processScroll(float Yoffset){
        Distance -= Yoffset * SCROLLSENS;
        if(Distance < 2.0f)
            Distance = 2.0f;
        else if(Distance > 10.0f)
            Distance = 10.0f;
    }

    void frameUpdate(float deltaTime)
    {
        updateCameraVector();
    }

private:
    void updateCameraVector()
    {
        CameraFront = Target - CameraPos;

        CameraPos.x = cos(glm::radians(Yaw)) * Distance * cos(glm::radians(Pitch)) + Target.x;
        CameraPos.y = -sin(glm::radians(Pitch)) * Distance;
        CameraPos.z = sin(glm::radians(Yaw)) * Distance * cos(glm::radians(Pitch)) + Target.z;


        CameraRight = glm::normalize(glm::cross(GlobalUp, CameraFront));
        CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
    }
};