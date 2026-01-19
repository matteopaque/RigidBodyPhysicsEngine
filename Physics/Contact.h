//
// Created by Clef on 1/19/26.
//

#ifndef RIGIDBOXES_CONTACT_H
#define RIGIDBOXES_CONTACT_H
#include <glm/glm.hpp>
using vec3 = glm::vec3;

class Contact
{
public:
    vec3 contactPoint;
    vec3 contactNormal;
    float penetration;
};


#endif //RIGIDBOXES_CONTACT_H