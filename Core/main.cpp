#include <iostream>
#include "../glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../Core/Game.h"

int main()
{
    Game game;

    game.Init();
    game.startLoop();
    game.Exit();
    return 0;
}