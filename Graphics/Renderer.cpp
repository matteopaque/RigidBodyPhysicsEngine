//
// Created by Clef on 1/13/26.
//

#include "Renderer.h"

#include <complex>
#include <glm/ext/matrix_clip_space.hpp>
#include "SphereVertex.h"
#include <glm/ext/quaternion_trigonometric.hpp>


std::vector<float> Renderer::genBox()
{
    std::vector<float> cube = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

    };
    return cube;
}

void Renderer::Init(Shader & shader)
{
    // load primitive shapes
    auto boxTemp = genBox();
    box.InitLoadData(boxTemp.data(), boxTemp.size()*sizeof(float));

    SphereVertex sphereVertex;
    sphereVertex.buildVertices(50, 50);
    sphere.InitLoadData(sphereVertex.getVertices(), sphereVertex.getVerticesSize()*sizeof(float), sphereVertex.getIndices(),
        sphereVertex.getIndicesSize()*sizeof(unsigned int));

    std::vector<float> planeData{
        1.f, 0.f, 1.f,
        1.f, 0.f, -1.f,
        -1.f, 0.f, 1.f,
        -1.f, 0.f, -1.f,
        1.f, 0.f, -1.f,
        -1.f, 0.f, 1.f
    };
    plane.InitLoadData(planeData.data(), planeData.size()*sizeof(float));


    setShader(shader);
    setPerspectiveProjectionMatrix(45.f);

}
void Renderer::setPerspectiveProjectionMatrix(float Fov)
{
    shader->use();
    projectionMatrix = glm::perspective(Fov,  4.f/3.f, 0.1f, 100.f);
    shader->setMat4("projection", projectionMatrix);
}


void Renderer::setViewMatrix(mat4 matrix)
{
    viewMatrix = matrix;
    shader->use();
    shader->setMat4("view", viewMatrix);
}
void Renderer::setShader(Shader& inShader)
{
    shader = &inShader;
}


void Renderer::draw(const Particle& particle)
{
    glm::vec3 position = particle.getPosition();
    mat4 model = glm::translate(mat4(1.f), position);
    model = glm::scale(model, glm::vec3(1.f)*particle.radius);
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("view", viewMatrix);
    shader->setMat4("projection", projectionMatrix);
    sphere.draw();
}
// does not draw the full plane
void Renderer::draw(const Plane& toDraw)
{
    const vec3 position = toDraw.position;
    mat4 model = glm::translate(mat4(1.f), position);
    if (glm::length(glm::cross({0.f, 1.f, 0.f}, toDraw.normal)) != 0.f)
        model = glm::rotate(model, glm::acos(glm::dot({0.f, 1.f, 0.f}, toDraw.normal)), glm::normalize(glm::cross({0.f, 1.f, 0.f}, toDraw.normal)));
    else
        model = glm::rotate(model, glm::acos(glm::dot({0.f, 1.f, 0.f}, toDraw.normal)), {1.f, 0.f, 0.f});
    model = glm::scale(model, glm::vec3(10.f));
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("view", viewMatrix);
    shader->setMat4("projection", projectionMatrix);
    plane.draw();
}

void Renderer::draw(RigidBody& toDraw)
{
    toDraw.calculateDerivedData();
    mat4 model = toDraw.modelMatrix;
    model = glm::scale(model, glm::vec3(2.f*toDraw.semiWidthx, 2.f*toDraw.semiHeighty, 2.f*toDraw.semiLengthZ));
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("view", viewMatrix);
    shader->setMat4("projection", projectionMatrix);
    box.draw();
}

