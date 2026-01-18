//
// Created by Clef on 1/13/26.
//

#ifndef RIGIDBOXES_RENDERER_H
#define RIGIDBOXES_RENDERER_H
#include <vector>
#include "Particle.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>
#include "Shader.h"
#include "Plane.h"

using mat4 = glm::mat4;

class Renderer
{
private:
    VertexArray box;
    VertexArray sphere;
    VertexArray plane;

    mat4 viewMatrix;
    mat4 projectionMatrix;

    Shader *shader;
public:
    Renderer(){};
    void Init(Shader & shader);
    static std::vector<float> genBox();

    void setShader(Shader& shader);
    void setViewMatrix(mat4);
    void setPerspectiveProjectionMatrix(float Fov);
    void draw(const Particle& particle);
    void draw(const Plane & toDraw);

};


#endif //RIGIDBOXES_RENDERER_H