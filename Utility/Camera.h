//
// Created by Clef on 1/15/26.
//

#ifndef RIGIDBOXES_CAMERA_H
#define RIGIDBOXES_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using vec3 = glm::vec3;

class Camera
{
public:
    vec3 position = {0.f, 0.f, 0.f};
    vec3 direction = {0.f, 0.f, -1.f};
    vec3 up = {0.f,1.f,0.f};

    glm::mat4 makeViewMatrix();

};


#endif //RIGIDBOXES_CAMERA_H