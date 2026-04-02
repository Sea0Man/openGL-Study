#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

enum movement_direction{
    FRONT,
    BACK,
    RIGHT,
    LEFT
};

const float SPEED = 4.0f;
const glm::vec3 SCALE = glm::vec3(1.0f);


class Player
{
public:
    glm::vec3 Position;
    glm::vec3 FrontDirection;
    glm::vec3 RightDirection;
    glm::vec3 GlobalUp;
    glm::vec3 Scale;

    glm::mat4 Model;

    float Speed;
    float Yaw;


    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 frontDirection = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f), glm::mat4 model = glm::mat4(1.0f)) : Scale(SCALE), Speed(SPEED), Yaw(YAW)
    {
        Position = position;
        FrontDirection = frontDirection;
        GlobalUp = globalUp;
        Model = model;

        updateModel();
        updatePlayerVector();
    }

    void updateDirection(glm::vec3 direction)
    {
        direction.y = 0.0f;
        FrontDirection = glm::normalize(direction);
        Yaw = (float)atan2(FrontDirection.z, FrontDirection.x);
    }

    void updateModel()
    {
        Model = glm::mat4(1.0f);
        Model = glm::scale(Model, Scale);
        Model = glm::translate(Model, Position);
        Model = glm::rotate(Model, -Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void processKeyboard(movement_direction direction, float deltaTime){
        float velocity = Speed * deltaTime;
        
        if(direction == FRONT)
            Position += glm::vec3(FrontDirection.x, 0.0f, FrontDirection.z) * velocity;
        if(direction == BACK)
            Position -= glm::vec3(FrontDirection.x, 0.0f, FrontDirection.z) * velocity;
        if(direction == RIGHT)
            Position -= glm::vec3(RightDirection.x, 0.0f, RightDirection.z) * velocity;
        if(direction == LEFT)
            Position += glm::vec3(RightDirection.x, 0.0f, RightDirection.z) * velocity;
        updateModel();
    }

    void frameUpdate(float deltaTime)
    {
        updatePlayerVector();
    }
private:
    void updatePlayerVector()
    {
        RightDirection = glm::normalize(glm::cross(GlobalUp, FrontDirection));
    }
};