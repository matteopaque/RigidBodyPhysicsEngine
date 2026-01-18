//
// Created by Clef on 1/13/26.
//

#include "../Core/Game.h"

#include <iostream>
#include <ranges>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (height > width*(600.f/800.f))
        glViewport(0,(height-width*(600.f/800.f))/2.f, width, width*(600.f/800.f));
    else
        glViewport((width - height*(800.f/600.f))/2.f,0, height*(800.f/600.f), height);
}
void Game::GraphicsInit()
{
    glfwInit();
    window = glfwCreateWindow(800, 600, "rigid Bodies", nullptr, nullptr);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,3 );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    shader.SetShader("Resources/vertexShader.glsl", "Resources/fragmentShader.glsl");
    renderer.Init(shader);
}



void Game::Init()
{
    GraphicsInit();

    timer.addTask(0.4f, [&](int TimesCalledBefore)
    {
        auto particle = Particle({0.f, 4.f, -5.f});
        particle.addMomentum(glm::vec3({4.f, 0.f, 0.f})*particle.getMass());
        registry.addParticle(std::move(particle));
        return true;
    });
    plane.position = {0.f, -4.f, 0.f};
    collisionManager.addObject(plane);
}

void Game::Exit()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::processInput(float deltaTime)
{
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
    firstPersonController.handleInput(window, deltaTime);
    double x,y;
    glfwGetCursorPos(window, &x, &y);
    firstPersonController.cursorHandle(window, x, y);
}

void Game::printOutput()
{
    glClearColor(0.f, 0.f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.setViewMatrix(camera.makeViewMatrix());
    const auto & particles = registry.getParticles();

    for (const auto &particle : particles | std::views::values)
    {
        renderer.draw(particle);
    }
    renderer.draw(plane);

    const char *error;
    std::cout << glfwGetError(&error) << " ";
    std::cout << error << "\n";


    glfwSwapBuffers(window);
}

void Game::updateWorld(float deltaTime)
{
    //particle.getPosition() += vec3({0.f, 0.f, -1.f}) *(float) deltaTime;
    timer.update(deltaTime);
    registry.update(deltaTime);
    collisionManager.findCollisions();
    collisionManager.resolveCollisions(deltaTime);
}

void Game::startLoop()
{
    float deltaTime,current,last;
    current = glfwGetTime();
    last = current - 0.016f;
    while (!glfwWindowShouldClose(window))
    {
        current = glfwGetTime();
        deltaTime = current - last;
        last = current;

        processInput(deltaTime);
        updateWorld(deltaTime);
        printOutput();
        while (glfwGetTime()-current < 1.f/60.f){}
    }

}


