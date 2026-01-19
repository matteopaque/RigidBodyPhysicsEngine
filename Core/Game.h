//
// Created by Clef on 1/13/26.
//

#ifndef RIGIDBOXES_GAME_H
#define RIGIDBOXES_GAME_H
#include "../glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Utility/Camera.h"
#include "../Physics/CollisionManager.h"
#include "../Utility/firstPersonCameraController.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Shader.h"
#include "../Physics/Particle.h"
#include "../Physics/physicsRegistry.h"
#include "../Utility/Timer.h"
#include "../Physics/RigidBody.h"

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
    CollisionManager collisionManager;

public:
    Game() : firstPersonController(camera, 0.1f), collisionManager(registry)
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