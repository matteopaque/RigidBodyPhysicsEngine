//
// Created by Clef on 1/18/26.
//

#include "CollisionManager.h"

#include <complex>
#include <ranges>


CollisionManager::CollisionManager(physicsRegistry& PhysicsRegistry)
{
    registry = &PhysicsRegistry;
}

void CollisionManager::findParticleCollisions()
{
    auto &Particles = registry->getParticles();
    for (auto i = Particles.begin(); i != Particles.end(); i++)
    {
        auto & particle = i->second;
        for (auto j = std::next(i); j!=Particles.end(); j++)
        {
            auto & particle2 = j->second;
            if (auto contact = ParticleAndParticle(particle, particle2))
                ParticleContacts.push_back(contact.value());
        }
        for (auto & collidableObject : collidableObjects | std::views::values)
        {
            if (auto planePointer = std::get_if<Plane*>(&collidableObject))
            {
                auto & plane = **planePointer;
                if (auto contact = ParticleAndPlane(particle, plane))
                    ParticleContacts.push_back(contact.value());
            }
        }
    }
}

void CollisionManager::findCollisions()
{
    findParticleCollisions();
}

void CollisionManager::resolveCollisions(float deltaTime)
{
    for (auto & contact : ParticleContacts)
    {
        contact.resolve(deltaTime);
    }
    ParticleContacts.clear();
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
    auto radiusSum = p1.radius + p2.radius;
    auto distance = glm::length(p1.getPosition()-p2.getPosition());
    auto normalVector = glm::normalize(p1.getPosition() - p2.getPosition());
    auto penetration = radiusSum - distance;
    if (penetration <= 0)
        return {};
    return ParticleContact(&p1, &p2, 0.5, normalVector, penetration);
}

std::optional<ParticleContact> CollisionManager::ParticleAndPlane(Particle& p1, Plane& plane)
{
    vec3 closestPointToParticle = p1.getPosition()- plane.normal*(glm::dot((p1.getPosition()-plane.position), plane.normal));
    if (glm::length(closestPointToParticle-p1.getPosition()) > p1.radius)
        return {};
    float penetration = p1.radius - glm::length(closestPointToParticle-p1.getPosition());
    ParticleContact contact(&p1, nullptr, 0.4f, plane.normal, penetration);
    return contact;
}

ContactData CollisionManager::BoxAndPlane(RigidBody& r1, Plane& plane)
{
    ContactData out;
    float x = r1.semiWidthx, y = r1.semiHeighty, z= r1.semiLengthZ;
    std::vector<vec3> Vertices{
            {x, y, z},
            {x, y, -z},
            {x, -y, z},
            {x, -y, -z},
            {-x, y, z},
            {-x, y, -z},
            {-x, -y, z},
            {-x, -y, -z}
    };
    r1.calculateDerivedData();
    for (auto & vertex : Vertices)
    {
        vertex = vec3(r1.modelMatrix* glm::vec4(vertex, 1.f));
    }
    for (auto & vertex : Vertices)
    {
        float vertexDistance = glm::dot(vertex, plane.normal);
        if (vertexDistance < plane.getOffset())
        {
            Contact contact{};
            contact.contactPoint = plane.normal * (1.f/2.f)*(plane.getOffset()-vertexDistance);
            contact.contactPoint += vertex;
            contact.contactNormal = plane.normal;
            contact.penetration = plane.getOffset() - vertexDistance;
            out.push_back(std::move(contact));
        }
    }
    return out;
}


ContactData CollisionManager::detectContacts(RigidBody& r1, RigidBody& r2)
{

}
