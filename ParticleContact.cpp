//
// Created by Clef on 1/18/26.
//

#include "ParticleContact.h"


ParticleContact::ParticleContact(Particle* p1, Particle* p2, float restitution, vec3 ContactNormal, float penetration):
p1(p1), p2(p2), restitution(restitution), ContactNormal(ContactNormal), penetration(penetration)
{
}

void ParticleContact::resolve(float deltaTime)
{
    resolveVelocity(deltaTime);
}

float ParticleContact::calculateSeparatingVelocity() const
{
    float seperatingVelocity;
    if (p2)
        seperatingVelocity = glm::dot(ContactNormal, p1->getVelocity() - p2->getVelocity());
    else
        seperatingVelocity = glm::dot(ContactNormal, p1->getVelocity());
    return seperatingVelocity;
}
// contact vector points at p1
void ParticleContact::resolveVelocity(float deltaTime)
{
    const float separatingVelocity = calculateSeparatingVelocity();
    if (separatingVelocity > 0)
        return;
    float newSeparatingVelocity = -separatingVelocity*restitution;

    float deltaVelocity = newSeparatingVelocity-separatingVelocity;
    float inverseMass = p1->getInverseMass();
    if (p2) inverseMass += p2->getInverseMass();
    if (inverseMass <= 0)
        return;
    float  impulse = deltaVelocity / inverseMass;
    vec3 impulseVector = impulse*ContactNormal;
    p1->addMomentum(impulseVector);
    if (p2)
        p2->addMomentum(impulseVector*-1.f);
}

void ParticleContact::resolvePenetration(float deltaTime)
{

}
