cmake_minimum_required(VERSION 4.0)
project(rigidBoxes)

set(CMAKE_CXX_STANDARD 20)
add_library(glad ${PROJECT_SOURCE_DIR}/glad/include/glad/glad.h ${PROJECT_SOURCE_DIR}/glad/src/glad.c
        Timer.cpp
        Timer.h
        firstPersonCameraController.cpp
        firstPersonCameraController.h
        Plane.h
        ParticleContact.cpp
        ParticleContact.h
        CollisionManager.cpp
        CollisionManager.h)
find_package(glfw3)
find_package(glm)

include_directories(${PROJECT_SOURCE_DIR}/glad/include)


add_executable(rigidBoxes main.cpp
        Game.cpp
        Game.h
        Renderer.cpp
        Renderer.h
        VertexArray.cpp
        VertexArray.h
        TextureArrangement.cpp
        TextureArrangement.h
        Shader.cpp
        Shader.h
        Particle.cpp
        Particle.h
        forceGenerator.h
        physicsRegistry.cpp
        physicsRegistry.h
        SphereVertex.cpp
        SphereVertex.h
        Camera.cpp
        Camera.h)
target_link_libraries(rigidBoxes glfw)
target_link_libraries(rigidBoxes glad)
target_link_libraries(rigidBoxes glm::glm)

file(COPY ${PROJECT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
