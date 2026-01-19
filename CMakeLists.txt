cmake_minimum_required(VERSION 4.0)
project(rigidBoxes)

set(CMAKE_CXX_STANDARD 20)
add_library(glad ${PROJECT_SOURCE_DIR}/glad/include/glad/glad.h ${PROJECT_SOURCE_DIR}/glad/src/glad.c)
find_package(glfw3)
find_package(glm)

include_directories(${PROJECT_SOURCE_DIR}/glad/include)


add_executable(rigidBoxes Core/main.cpp
        Core/Game.cpp
        Core/Game.h
        Graphics/Renderer.cpp
        Graphics/Renderer.h
        Graphics/VertexArray.cpp
        Graphics/VertexArray.h
        Graphics/TextureArrangement.cpp
        Graphics/TextureArrangement.h
        Graphics/Shader.cpp
        Graphics/Shader.h
        Physics/Particle.cpp
        Physics/Particle.h
        Physics/forceGenerator.h
        Physics/physicsRegistry.cpp
        Physics/physicsRegistry.h
        Graphics/SphereVertex.cpp
        Graphics/SphereVertex.h
        Utility/Camera.cpp
        Utility/Timer.cpp
        Utility/Timer.h
        Utility/firstPersonCameraController.cpp
        Utility/firstPersonCameraController.h
        Utility/Plane.h
        Physics/ParticleContact.cpp
        Physics/ParticleContact.h
        Physics/CollisionManager.cpp
        Physics/CollisionManager.h
        Utility/Camera.h
        Physics/RigidBody.cpp
        Physics/RigidBody.h
        Physics/Contact.cpp
        Physics/Contact.h)
target_link_libraries(rigidBoxes glfw)
target_link_libraries(rigidBoxes glad)
target_link_libraries(rigidBoxes glm::glm)

file(COPY ${PROJECT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
