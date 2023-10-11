#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <iostream>

#ifdef CAMERA_MOVEMENT
#define CAMERA_CONSTRAINE_PITCH
#define CAMERA_ENABLE_UP_DOWN
#define CAMERA_ENABLE_ZOOM
#endif
#ifdef CAMERA_FLY
#define CAMERA_ENABLE_UP_DOWN
#define CAMERA_ENABLE_ROLL
#endif
#ifdef CAMERA_FPS
#define CAMERA_CONSTRAINE_PITCH
#define CAMERA_KEEP_ON_PLAIN
#endif
#ifndef CAMERA_FPS or CAMERA_FLY or CAMERA_MOVEMENT or CAMERA_NO_FEATURES
#define CAMERA_CONSTRAINE_PITCH
#define CAMERA_ENABLE_UP_DOWN
#define CAMERA_ENABLE_ROLL
#define CAMERA_ENABLE_ZOOM
#endif

enum CameraMovement {
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 4,
    RIGHT = 8,

#ifdef CAMERA_ENABLE_UP_DOWN
    UP = 16,
    DOWN = 32,
#endif // CAMERA_ENABLE_UP_DOWN

#ifdef CAMERA_ENABLE_ROLL
    ROLL_LEFT = 64,
    ROLL_RIGHT = 128
#endif // CAMERA_ENABLE_ROLL
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

/// <summary>
/// 
/// Init a basic Camera 
/// <para>Following Presets are avaiable the default is all features activated:</para>
/// #CAMERA_MOVEMENT
/// #CAMERA_FLY
/// #CAMERA_FPS
/// CAMERA_NO_FEATURES
/// 
/// </summary>
class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw = YAW;
    float Pitch = PITCH;
    float MovementSpeed = SPEED;
    float RollSpeed = SPEED * 20;
    float MouseSensitivity = SENSITIVITY;
    float Fov = FOV;

#ifdef CAMERA_ENABLE_ROLL
    float Roll = ROLL;
#endif // CAMERA_ENABLE_ROLL

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
        : Position(position), WorldUp(up), Front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        const float speed = MovementSpeed * deltaTime;
#ifdef CAMERA_KEEP_ON_PLAIN
        float pos = Position.y;
        glm::vec3 front = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
        glm::vec3 right = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));
#else
        glm::vec3 front = Front;
        glm::vec3 right = Right;
#endif // CAMERA_KEEP_ON_PLAIN
        if (direction & FORWARD)
        {
            Position += front * speed;
        }
        if (direction & BACKWARD)
        {
            Position -= front * speed;
        }
        if (direction & LEFT)
        {
            Position -= right * speed;
        }
        if (direction & RIGHT)
        {
            Position += right * speed;
        }
#ifdef CAMERA_KEEP_ON_PLAIN
        Position.y = pos;
#endif // CAMERA_KEEP_ON_PLAIN

#ifdef CAMERA_ENABLE_UP_DOWN
        if (direction & UP)
        {
            Position += Up * speed;
        }
        if (direction & DOWN)
        {
            Position -= Up * speed;
        }
#endif // CAMERA_ENABLE_UP_DOWN

#ifdef CAMERA_ENABLE_ROLL
        if (direction & ROLL_RIGHT)
        {
            Roll += RollSpeed * deltaTime;
            updateCameraUpRight();
        }
        if (direction & ROLL_LEFT)
        {
            Roll -= RollSpeed * deltaTime;
            updateCameraUpRight();
        }
#endif // CAMERA_ENABLE_ROLL
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

#ifdef CAMERA_ENABLE_ROLL
        Yaw += xoffset * cos(glm::radians(Roll)) - yoffset * sin(glm::radians(Roll));
        Pitch -= yoffset * cos(glm::radians(Roll)) + xoffset * sin(glm::radians(Roll));
#else
        Yaw += xoffset;
        Pitch -= yoffset;
#endif // CAMERA_ENABLE_ROLL

#ifdef CAMERA_CONSTRAINE_PITCH
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
#endif // CAMERA_CONSTRAINE_PITCH

        updateCameraVectors();
    }

#ifdef CAMERA_ENABLE_ZOOM
    void ProcessMouseScroll(float yoffset)
    {
        Fov -= (float)yoffset;
        if (Fov < 1.0f)
        {
            Fov = 1.0f;
        }
        if (Fov > 45.0f)
        {
            Fov = 45.0f;
        }
    }
#endif // CAMERA_ENABLE_ZOOM

    void updateCameraVectors()
    {
        updateCameraFront();
        updateCameraUpRight();
    }

private:
    void updateCameraFront()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(direction);
    }

    void updateCameraUpRight()
    {
#ifdef CAMERA_ENABLE_ROLL
        glm::vec3 up = glm::cross(glm::cross(Front, WorldUp), Front);
        Up = glm::normalize(glm::rotate(glm::angleAxis(glm::radians(Roll), Front), up));
        Right = glm::cross(Front, Up);
#else
        Right = glm::cross(Front, WorldUp);
        Up = glm::cross(Right, Front);
#endif // CAMERA_ENABLE_ROLL
    }
};
