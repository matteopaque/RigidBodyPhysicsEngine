//
// Created by Clef on 1/18/26.
//

#include "CollisionManager.h"

#include <ranges>


CollisionManager::CollisionManager(physicsRegistry& PhysicsRegistry)
{
    registry = &PhysicsRegistry;
}

void CollisionManager::findCollisions()
{
    auto &Particles = registry->getParticles();
    for (auto & particle : Particles | std::views::values)
    {
        for (auto & collidableObject : collidableObjects | std::views::values)
        {
            if (auto planePointer = std::get_if<Plane*>(&collidableObject))
            {
                auto & plane = **planePointer;
                if (auto contact = ParticleAndPlane(particle, plane))
                    contacts.push_back(contact.value());
            }
        }
    }
}

void CollisionManager::resolveCollisions(float deltaTime)
{
    for (auto & contact : contacts)
    {
        contact.resolve(deltaTime);
    }
    contacts.clear();
}

cID CollisionManager::addObject(CollidableObject& toAdd)
{
    collidableObjects[topID] = toAdd;
    const auto id = topID;
    topID++;
    return id;
}

void CollisionManager::removeObject(cID toRemove)
{
    erase_if(collidableObjects, [toRemove](auto pair)
    {
        return pair.first == toRemove;
    });
}

std::optional<ParticleContact> CollisionManager::ParticleAndParticle(Particle& p1, Particle& p2)
{
}

std::optional<ParticleContact> CollisionManager::ParticleAndPlane(Particle& p1, Plane& plane)
{
    vec3 closestPointToParticle = p1.getPosition()- plane.normal*(glm::dot((p1.getPosition()-plane.position), plane.normal));
    if (glm::length(closestPointToParticle-p1.getPosition()) > p1.radius)
        return {};
    float penetration = p1.radius - glm::length(closestPointToParticle-p1.getPosition());
    ParticleContact contact(&p1, nullptr, 0.90f, plane.normal, penetration);
    return contact;
}
