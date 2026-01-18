//
// Created by Clef on 1/15/26.
//

#include "Particle.h"

#include "physicsRegistry.h"

Particle::Particle(vec3 position) : position(position)
{
    inverseMass = 1.f;
    radius = 1.f;
}
Particle::Particle()
{
}
void Particle::addMomentum(vec3 toAdd)
{
    velocity += toAdd* inverseMass;
}
vec3 Particle::getVelocity() const
{
    return velocity;
}
float Particle::getMass()
{
    return 1.f/inverseMass;
}


float Particle::getInverseMass()
{
    return inverseMass;
}




vec3 Particle::getPosition() const
{
    return position;
}
vec3& Particle::getPosition()
{
    return position;
}

Particle::Particle(vec3 position, float mass, float radius) : position(position), radius(radius)
{
    inverseMass = 1.f/mass;
}

void Particle::setMass(float mass)
{
    inverseMass = 1.f/mass;
}

void Particle::addForce(vec3 toAdd)
{
    forceAccul += toAdd;
}


void Particle::integrate(float deltaTime)
{
    position += velocity * deltaTime;
    vec3 toAccelerate = acceleration + forceAccul*inverseMass;
    velocity += toAccelerate * deltaTime;
    forceAccul = {0.f, 0.f, 0.f};
}


