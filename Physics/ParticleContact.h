//
// Created by Clef on 1/18/26.
//

#ifndef RIGIDBOXES_PARTICLECONTACT_H
#define RIGIDBOXES_PARTICLECONTACT_H
#include "Particle.h"
#include "physicsRegistry.h"

class ParticleContact
{
public:
    Particle * p1;
    Particle * p2;
    float restitution;
    float penetration;
    vec3 ContactNormal;
    ParticleContact(Particle * p1, Particle * p2, float restitution, vec3 ContactNormal, float penetration);
    void resolve(float deltaTime);
protected:
    float calculateSeparatingVelocity() const;
private:
    void resolveVelocity(float deltaTime);
    void resolvePenetration(float deltaTime);
};


#endif //RIGIDBOXES_PARTICLECONTACT_H