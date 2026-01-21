//
// Created by Clef on 1/19/26.
//

#include "RigidBody.h"

#include <iostream>

void RigidBody::calculateDerivedData()
{
    modelMatrix = glm::translate(mat4(1.f), position);
    modelMatrix = modelMatrix * glm::mat4_cast(orientation);
}

void RigidBody::calculateWorldInverseInertiaTensor()
{
    mat3 orientMatrix = glm::inverse(glm::mat3_cast(orientation));
    worldInverseInertiaTensor = orientMatrix * inverseInertiaTensor * glm::inverse(orientMatrix);
}

void RigidBody::setInertiaTensor(const mat3& inertiaTensor)
{
    inverseInertiaTensor = glm::inverse(inertiaTensor);
}

void RigidBody::addForce(const vec3 force)
{
    forceAccum += force;
}

void RigidBody::addForceAtPoint(const vec3 force, const vec3 worldPoint)
{
    forceAccum += force;
    torqueAccum += glm::cross(worldPoint-position, force);
}

void RigidBody::addForceAtLocalPoint(const vec3 force, const vec3 localPoint)
{
    calculateDerivedData();
    forceAccum += force;
    torqueAccum += glm::cross(glm::vec3(modelMatrix * glm::vec4(localPoint,1.f))-position, forceAccum);
}

void RigidBody::clearAccumulators()
{
    forceAccum = {0.f, 0.f, 0.f};
    torqueAccum = {0.f, 0.f, 0.f};
}

void RigidBody::integrate(float deltaTime)
{
    position += velocity * deltaTime;
    if (glm::length(rotation) > 0.02f)
        orientation = glm::rotate(orientation, glm::length(rotation)*deltaTime, glm::normalize(rotation));
    calculateWorldInverseInertiaTensor();
    auto toAccel = acceleration;
    toAccel += forceAccum*inverseMass;
    auto AngularAcceleration = worldInverseInertiaTensor* torqueAccum;
    velocity += toAccel * deltaTime;
    rotation += AngularAcceleration*deltaTime;
    velocity *= std::powf(dampening, deltaTime);
    rotation *= std::powf(angularDampening, deltaTime);


    calculateDerivedData();
    clearAccumulators();
}

void RigidBody::setBoxInertiaTensor()
{
    float mass = 1.f/inverseMass;
    mat3 tensor(1.f);
    tensor[0][0] = mass*(1.f/12.f)*(std::powf((semiHeighty*2.f),2.f)+std::powf((semiLengthZ*2.f),2));
    tensor[1][1] = mass*(1.f/12.f)*(std::powf((semiWidthx*2.f),2.f)+std::powf((semiLengthZ*2.f),2));
    tensor[2][2] = mass*(1.f/12.f)*(std::powf((semiWidthx*2.f),2.f)+std::powf((semiHeighty*2.f),2));


    inverseInertiaTensor = glm::inverse(tensor);
}

// x, y, then z
vec3 RigidBody::getAxis(unsigned index)
{
    switch (index)
    {
    case 0:
        return glm::mat3_cast(orientation) * vec3 ({1.f, 0.f, 0.f});
    case 1:
        return glm::mat3_cast(orientation) * vec3 ({0.f, 1.f, 0.f});
    case 2:
        return glm::mat3_cast(orientation) * vec3 ({0.f, 0.f, 1.f});
    }
    return vec3({0.f, 0.f, 0.f});
}


void RigidBody::addAngularMomentumAtWorldPoint(glm::vec3 momentum, glm::vec3 worldPoint)
{
    velocity += inverseMass*momentum;
    auto angularMoment = glm::cross(worldPoint - position, momentum);
    calculateWorldInverseInertiaTensor();
    rotation += worldInverseInertiaTensor * angularMoment;
}
