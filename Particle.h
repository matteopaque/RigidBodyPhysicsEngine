//
// Created by Clef on 1/15/26.
//

#ifndef RIGIDBOXES_PARTICLE_H
#define RIGIDBOXES_PARTICLE_H
#include <glm/glm.hpp>
using vec3 = glm::vec3;

class Particle
{
    vec3 position = {0.f, 0.f, 0.f};
    vec3 velocity = {0.f, 0.f, 0.f};
    vec3 acceleration = {0.f, -10.f, 0.f};
    vec3 forceAccul = {0.f, 0.f, 0.f};
    float inverseMass = 1;
public:
    float radius = 1;
    // clears forces
    Particle();
    explicit Particle(vec3 position);
    Particle(vec3 position, float mass, float radius);
    void setMass(float mass);
    void integrate(float deltaTime);
    void addForce(vec3 toAdd);
    vec3 getPosition() const;
    vec3 &getPosition();
    void addMomentum(vec3 toAdd);
    vec3 getVelocity() const;
    float getInverseMass();
    float getMass();

};


#endif //RIGIDBOXES_PARTICLE_H