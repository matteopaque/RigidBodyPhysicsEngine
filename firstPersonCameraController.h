//
// Created by Clef on 1/17/26.
//

#ifndef RIGIDBOXES_FIRSTPERSONCAMERA_H
#define RIGIDBOXES_FIRSTPERSONCAMERA_H
#include "Camera.h"
#include <GLFW/glfw3.h>

class firstPersonCameraController
{
    Camera & camera;
    float lastX= 400,lastY = 300;
    bool FirstMouse = true;
    float yaw = 0, pitch = 0;
    const float sensitivity;

public:
    explicit firstPersonCameraController(Camera & camera, const float sensitivity);
    void cursorHandle(GLFWwindow * window, double xpos, double ypos);
    void handleInput(GLFWwindow * window, float deltaTime);
};


#endif //RIGIDBOXES_FIRSTPERSONCAMERA_H