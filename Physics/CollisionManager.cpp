//
// Created by Clef on 1/18/26.
//

#include "CollisionManager.h"

#include <algorithm>
#include <complex>
#include <iostream>
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
    std::cout << "collision\n";
    findParticleCollisions();
    auto & rigidBodies = registry->getRigidBodies();
    for (auto i = rigidBodies.begin(); i != rigidBodies.end(); i++)
    {
        for (auto j = std::next(i); j!= rigidBodies.end(); j++)
        {
            auto & r1 = i->second;
            auto & r2 = j->second;
            if (auto index = std::find_if(contacts.begin(), contacts.end(), [&](RigidCollision& collision)
            {
                auto & rr1 = *collision.pair.first;
                auto *p = std::get_if<RigidBody*>(&collision.pair.second);
                if (!p)
                    return false;
                auto & rr2 = **p;
                return rr1 == r1 && rr2 == r2;
            });
                index != contacts.end()
                )
            {
                updateCollision(*index);
                continue;
            }
            RigidCollision collision;
            collision.friction = 0.f;
            collision.restitution = 0.1f;
            collision.pair.first = &r1;
            collision.pair.second = &r2;
            if (BoxAndBox(collision))
            {
                std::cout << collision.data.at(0).point.y << " woohoo\n";
                contacts.push_back(collision);
            }
        }
        for (auto & collidable : collidableObjects | std::views::values)
        {
            auto & plane = *std::get<Plane*>(collidable);
            auto & r1 = i->second;
            if (auto index = std::find_if(contacts.begin(), contacts.end(), [&](RigidCollision& collision)
            {
                auto & rr1 = *collision.pair.first;
                auto *p = std::get_if<CollidableObject>(&collision.pair.second);
                if (!p)
                    return false;
                auto *p2 = std::get_if<Plane*>(p);
                if (!p2)
                    return false;
                auto & inPlane = **p2;
                return rr1 == r1 && plane == inPlane;
            }); index !=contacts.end())
            {
                index->data = BoxAndPlane(r1, plane);
                continue;
            }
            auto contact = BoxAndPlane(r1, plane);
            if (contact.empty())
                continue;
            auto collision = RigidCollision();
            collision.pair.first = &r1;
            collision.pair.second = &plane;
            collision.data = contact;
            collision.restitution = 0.1f;
            collision.friction = 1.f;
            contacts.push_back(collision);
        }
    }
}
void makeOrthonormalBasis(vec3 & x, vec3 & y, vec3& z)
{
    z = glm::cross(x, y);
    if (z.length() == 0.f)
        z = glm::cross(x, {1.f, 0.f, 0.f});
    y = glm::cross(z, x);

    y = glm::normalize(y);
    z = glm::normalize(z);

}

void CollisionManager::resolveCollisions(float deltaTime)
{
    for (auto & contact : ParticleContacts)
    {
        contact.resolve(deltaTime);
    }
    const int iterAmount = 4;
    const float inverseIterAmount = 1.f/iterAmount;
    for (int i = 0; i< iterAmount; i++){

        erase_if(contacts, [](RigidCollision& collision)
    {
        return collision.data.empty();
    });
    for (auto & collision : contacts)
    {
        for (auto & contact : collision.data)
        {

            int count = collision.data.size();
            float inverseCount = 1.f/count;
            auto & r1 = *collision.pair.first;
            auto  * p = std::get_if<RigidBody*>(&collision.pair.second);
            // handle contacts
            r1.calculateWorldInverseInertiaTensor();
            if (p)
            {
                contact.normal *= -1.f;
                auto & r2 = **p;

                auto relativePosition1 = contact.point - r1.position;
                auto relativePosition2 = contact.point - r2.position;

                auto velocity1 = r1.velocity + glm::cross(r1.rotation, relativePosition1);
                auto velocity2 = r2.velocity + glm::cross(r2.rotation, relativePosition2);

                auto relativeVelocity = velocity2 - velocity1;

                r2.calculateWorldInverseInertiaTensor();
                auto impulse = (-(1+collision.restitution)*glm::dot(relativeVelocity, contact.normal))/
                    (r1.inverseMass + r2.inverseMass+
                        glm::dot(
                            glm::cross(r1.worldInverseInertiaTensor * glm::cross(
                                relativePosition1,
                                contact.normal),
                                relativePosition1)
                        + glm::cross(
                            r2.worldInverseInertiaTensor *
                                glm::cross(
                                    relativePosition2,
                                    contact.normal),
                                    relativePosition2),
                                    contact.normal));
                r1.velocity += -impulse*r1.inverseMass*contact.normal*inverseIterAmount;
                r2.velocity += impulse*r2.inverseMass*contact.normal*inverseIterAmount;

                r1.rotation += -impulse*(r1.worldInverseInertiaTensor * glm::cross(relativePosition1, contact.normal))*inverseIterAmount;
                r2.rotation += impulse*(r2.worldInverseInertiaTensor * glm::cross(relativePosition2, contact.normal))*inverseIterAmount;

                r1.position += contact.normal * 0.5f*contact.penetration*inverseIterAmount;
                r2.position += -contact.normal * 0.5f*contact.penetration *inverseIterAmount;
                contact.normal *= -1.f;
                continue;
            }
            auto  p2 = std::get<CollidableObject>(collision.pair.second);
            auto & plane = *std::get<Plane*>(p2);





            auto relativePosition = contact.point - r1.position;


            auto impulse = (collision.restitution*glm::dot(r1.velocity+glm::cross(r1.rotation, relativePosition),contact.normal)
                        + glm::dot(r1.velocity, contact.normal) + glm::dot(r1.rotation, glm::cross(relativePosition, contact.normal)))/
                            (r1.inverseMass + glm::dot(r1.worldInverseInertiaTensor * glm::cross(relativePosition, contact.normal), glm::cross(relativePosition, contact.normal)));

            r1.rotation -= impulse* r1.worldInverseInertiaTensor* glm::cross(relativePosition, contact.normal)*inverseIterAmount;
            r1.velocity -= impulse*r1.inverseMass*contact.normal * inverseIterAmount;



            r1.position += contact.normal*contact.penetration *inverseIterAmount;

        }

    }
    }for (auto & collision : contacts)
    {
        updateCollision(collision);
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
    for (int i = 0; i < Vertices.size(); i++)
    {
        auto & vertex = Vertices.at(i);
        float vertexDistance = glm::dot(vertex, plane.normal);
        if (vertexDistance < plane.getOffset())
        {
            Contact contact{};
            contact.point = plane.normal * (1.f/2.f)*(plane.getOffset()-vertexDistance);
            contact.point += vertex;
            contact.normal = plane.normal;
            contact.penetration = plane.getOffset() - vertexDistance;
            contact.vertex = i;
            out.push_back(std::move(contact));
        }
    }
    return out;
}

std::optional<Contact> CollisionManager::BoxAndPlane(RigidBody& r1, Plane& plane, int index)
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
    if (index > 7 || index < 0)
        return {};
    auto & vertex = Vertices.at(index);
    return PlaneAndPoint(plane, vertex);
}

std::optional<Contact> CollisionManager::BoxAndBox(RigidBody& r1, RigidBody& r2, int index)
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
    if (index > 7 || index < 0)
        return {};
    auto vertex = Vertices.at(index);
    return BoxAndPoint(r2, vertex);
}




ContactData CollisionManager::detectContacts(RigidBody& r1, RigidBody& r2)
{

}

std::optional<Contact> CollisionManager::PlaneAndPoint(Plane& plane, vec3 point)
{
    Contact out;
    auto depth = glm::dot(plane.normal, point);
    if (depth > plane.getOffset())
    {
        return{};
    }
    out.normal = plane.normal;
    out.point = point;
    out.penetration = plane.getOffset() - depth;
    return out;
}

std::optional<Contact> CollisionManager::BoxAndPoint(RigidBody& r1, vec3 point)
{
    r1.calculateDerivedData();
    vec3 relativePoint = vec3(glm::inverse(r1.modelMatrix)*glm::vec4(point, 1.f));
    vec3 normal;

    float minDepth = r1.semiWidthx - glm::abs(relativePoint.x);
    if (minDepth < 0.05f) return {};
    normal = r1.getAxis(0)* ((relativePoint.x < 0)?-1.f:1.f);

    float depth = r1.semiHeighty - glm::abs(relativePoint.y);
    if (depth < 0.05) return {};
    if (depth < minDepth)
    {
        minDepth = depth;
        normal = r1.getAxis(1) *((relativePoint.y < 0)?-1.f:1.f);
    }
    depth = r1.semiLengthZ - glm::abs(relativePoint.z);
    if (depth < 0.05) return {};
    if (depth < minDepth)
    {
        minDepth = depth;
        normal= r1.getAxis(2) * ((relativePoint.z<0)?-1.f:1.f);
    }
    Contact contact;
    contact.point = point;
    contact.normal = normal;
    contact.penetration = minDepth;
    return contact;

}

bool CollisionManager::BoxAndBox(RigidCollision& data)
{
    auto & r1 = *data.pair.first;
    auto & r2 = *std::get<RigidBody*>(data.pair.second);
    Contact out;
    out.penetration = -1.f;
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
    x = r2.semiWidthx, y = r2.semiHeighty, z= r2.semiLengthZ;
    std::vector<vec3> Vertices2{
                    {x, y, z},
                    {x, y, -z},
                    {x, -y, z},
                    {x, -y, -z},
                    {-x, y, z},
                    {-x, y, -z},
                    {-x, -y, z},
                    {-x, -y, -z}
    };
    r2.calculateDerivedData();
    for (auto & vertex : Vertices2)
    {
        vertex = vec3(r2.modelMatrix* glm::vec4(vertex, 1.f));
    }

    for (int i = 0; i<Vertices.size(); i++)
    {
        auto &vertex = Vertices.at(i);
        auto  temp = BoxAndPoint(r2, vertex);
        if (temp)
        {
            if (temp->penetration > out.penetration)
            {
                out = temp.value();
                out.vertex = i;
            }
        }
    }
    bool shouldSwap = false;
    for (int i = 0; i<Vertices2.size(); i++)
    {
        auto & vertex = Vertices2.at(i);
        auto  temp = BoxAndPoint(r1, vertex);
        if (temp)
        {
            if (temp->penetration > out.penetration)
            {
                out = temp.value();
                out.vertex = i;
                shouldSwap = true;
            }
        }
    }
    //if (shouldSwap)
      //  std::swap(r1, r2);

    if (out.penetration < 0.f)
        return false;

    data.data.push_back(out);
    return true;
}
// before box and box
void CollisionManager::updateCollision(RigidCollision& data)
{
    std::vector<std::vector<Contact>::iterator> toRemove;
    for (auto index = data.data.begin(); index < data.data.end(); index++)
    {
        auto & contact = *index;
        auto vertex = contact.point;
        auto & r1 = *data.pair.first;
        if (auto p = std::get_if<RigidBody*>(&data.pair.second))
        {
            auto & r2 = **p;

                if (auto newContact = BoxAndBox(r1, r2, contact.vertex))
                {
                    contact = newContact.value();
                    continue;
                }

        }
        if (auto p = std::get_if<CollidableObject>(&data.pair.second))
        {
            if (auto p2 = std::get_if<Plane*>(p))
            {
                auto plane =**p2;
                if (auto newContact = BoxAndPlane(r1, plane, contact.vertex))
                {
                    contact = newContact.value();
                    continue;
                }
            }
        }
        toRemove.push_back(index);
    }
    for (auto index : toRemove)
    {
        data.data.erase(index);
    }
}

