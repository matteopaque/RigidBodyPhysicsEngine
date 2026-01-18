//
// Created by Clef on 1/13/26.
//

#ifndef RIGIDBOXES_GAME_H
#define RIGIDBOXES_GAME_H
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "firstPersonCameraController.h"
#include "Renderer.h"
#include "Shader.h"
#include "Particle.h"
#include "physicsRegistry.h"
#include "Timer.h"

class Game
{
private:
    GLFWwindow * window;
    Renderer renderer;
    Shader shader;
    Camera camera;
    physicsRegistry registry;
    Timer timer;
    firstPersonCameraController firstPersonController;
    Plane plane{};

public:
    Game() : firstPersonController(camera, 0.1f)
    {
    }

    void GraphicsInit();
    void Init();
    void startLoop();
    void Exit();

    void processInput(float deltaTime);
    void updateWorld(float deltaTime);
    void printOutput();


    // callbacks
};


#endif //RIGIDBOXES_GAME_H