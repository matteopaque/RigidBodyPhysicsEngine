//
// Created by Clef on 1/18/26.
//

#ifndef RIGIDBOXES_PLANE_H
#define RIGIDBOXES_PLANE_H
#include<glm/glm.hpp>
using vec3 = glm::vec3;

struct Plane
{
public:
    vec3 position = {0.f, 0.f, 0.f};
    vec3 normal = {0.f, 1.f, 0.f};
    float getOffset() const
    {
        return glm::dot(position, normal);
    };
};

// plane segment should be made by 1 plane and 4 bounding planes

#endif //RIGIDBOXES_PLANE_H