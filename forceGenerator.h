//
// Created by Clef on 1/15/26.
//

#ifndef RIGIDBOXES_FORCEGENERATOR_H
#define RIGIDBOXES_FORCEGENERATOR_H

#include "Particle.h"

class forceGenerator
{
public:
    virtual void addForce(Particle& particle) = 0;
    virtual ~forceGenerator();
};


#endif //RIGIDBOXES_FORCEGENERATOR_H