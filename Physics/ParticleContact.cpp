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
    resolvePenetration(deltaTime);
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

    vec3 Acceleration = p1->getAcceleration();
    if (p2)
        Acceleration - p2->getAcceleration();
    float accCausedSepVelocity = glm::dot(Acceleration, ContactNormal) * deltaTime;
    if (accCausedSepVelocity < 0)
    {
        newSeparatingVelocity -= accCausedSepVelocity * restitution;
        if (newSeparatingVelocity < 0) newSeparatingVelocity = 0;
    }


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
    if (penetration <= 0 )
        return;
    if (!p2)
    {
        p1->getPosition() += ContactNormal*penetration;
        return;
    }
    const float inverseMass1 = p1->getInverseMass();
    const float inverseMass2 = p2->getInverseMass();

    if (inverseMass2 == 0.f & inverseMass1 == 0.f)
        return;
    if (inverseMass1 == 0.f)
    {
        p2->getPosition() += -1.f*ContactNormal*penetration;
        return;
    }
    if (inverseMass2 == 0.f)
    {
        p1->getPosition() += ContactNormal*penetration;
    }
    const float Mass1 = 1.f/inverseMass1;
    const float Mass2 = 1.f/inverseMass2;
    const float deltaLength1 = penetration*(Mass2/(Mass1+Mass2));
    const float deltaLength2 = penetration*(Mass1/(Mass1+Mass2));
    const vec3 deltaPosition1 = ContactNormal*deltaLength1;
    const vec3 deltaPosition2 = ContactNormal*deltaLength2*(-1.f);
    p1->getPosition() += deltaPosition1;
    p2->getPosition() += deltaPosition2;
}
