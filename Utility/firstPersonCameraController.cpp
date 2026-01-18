//
// Created by Clef on 1/17/26.
//

#include "firstPersonCameraController.h"
#include <iostream>

firstPersonCameraController::firstPersonCameraController(Camera& camera, const float sensitivity) : camera(camera), sensitivity(sensitivity)
{
}

void firstPersonCameraController::cursorHandle(GLFWwindow* window, double xpos, double ypos)
{
        if (FirstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            FirstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        yaw += xoffset*sensitivity;
        pitch += yoffset*sensitivity;
        if (pitch > 89.f)
            pitch = 89.f;
        if (pitch < -89.f)
            pitch= -89.f;
        camera.direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        camera.direction.y = std::sin(glm::radians(pitch));
        camera.direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        camera.direction = glm::normalize(camera.direction);
    std::cout << yaw << "\n";
}

void firstPersonCameraController::handleInput(GLFWwindow* window, float deltaTime)
{
    glm::mat3 xzProjection {
        1, 0, 0,
        0, 0, 0,
        0, 0, 1};
    glm::vec3 frontLateral = glm::normalize(xzProjection * camera.direction);
    const float cameraSpeed = 5.f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.position += cameraSpeed *frontLateral;
    }
    if (glfwGetKey(window, GLFW_KEY_R)== GLFW_PRESS)
        camera.position -= cameraSpeed *frontLateral;
    if (glfwGetKey(window, GLFW_KEY_S)== GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(frontLateral, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(frontLateral, camera.up)) * -cameraSpeed;
}
