//
// Created by Clef on 1/15/26.
//

#include "Camera.h"

glm::mat4 Camera::makeViewMatrix()
{
    return glm::lookAt(position, position + direction, up);
}
