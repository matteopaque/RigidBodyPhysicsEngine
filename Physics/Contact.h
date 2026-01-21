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
    vec3 point;
    vec3 normal;
    float penetration;
    int vertex;
};


#endif //RIGIDBOXES_CONTACT_H