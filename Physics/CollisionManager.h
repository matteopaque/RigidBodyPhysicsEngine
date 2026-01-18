//
// Created by Clef on 1/18/26.
//

#ifndef RIGIDBOXES_COLLISIONMANAGER_H
#define RIGIDBOXES_COLLISIONMANAGER_H
#include <optional>
#include <variant>

#include "physicsRegistry.h"
#include "ParticleContact.h"
#include "../Utility/Plane.h"
using cID = unsigned int;
using CollidableObject = std::variant<Plane*>;
class CollisionManager
{
    std::map<cID, CollidableObject> collidableObjects;
    physicsRegistry* registry;
    std::vector<ParticleContact> contacts;
    cID topID = 0;

public:
    explicit CollisionManager(physicsRegistry& PhysicsRegistry);
    template<typename T>
    cID addObject(T& toAdd)
    {
        CollidableObject collidableObject {&toAdd};
        return addObject(collidableObject);
    }
    cID addObject(CollidableObject& toAdd);
    void removeObject(cID toRemove);
    void findCollisions();
    void resolveCollisions(float deltaTime);

private:
    std::optional<ParticleContact> ParticleAndParticle(Particle& p1, Particle& p2);
    std::optional<ParticleContact> ParticleAndPlane(Particle& p1, Plane& plane);

};


#endif //RIGIDBOXES_COLLISIONMANAGER_H