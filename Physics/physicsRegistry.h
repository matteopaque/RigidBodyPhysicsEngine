//
// Created by Clef on 1/15/26.
//

#ifndef RIGIDBOXES_PARTICLEREGISTRY_H
#define RIGIDBOXES_PARTICLEREGISTRY_H
#include "Particle.h"
#include "forceGenerator.h"
#include"RigidBody.h"
#include <map>
#include <memory>

using pID = unsigned int;
using gID = unsigned int;
using rID = unsigned int;
class physicsRegistry
{
    std::map<pID, Particle> Particles;
    std::map<rID, RigidBody> Bodies;
    std::map<gID, std::unique_ptr<forceGenerator>> Generators;
    std::vector<std::pair<pID,gID>> forceLinksParticle;
    std::vector<std::pair<rID,gID>> forceLinksBody;
    pID topPID = 0;
    gID topGID = 0;
    rID topRID = 0;
public:
    RigidBody & getRigidBody(rID bodyID);
    rID addRigidBody (RigidBody && body);
    void removeBody (rID toRemove);
    void createLinkBody(rID bodyID, gID generatorID);
    void removeLinkBody(rID bodyID, gID generatorID);

    auto& getRigidBodies()
    {
        return Bodies;
    }

    Particle & getParticle(pID particleID);
    void removeParticle(pID particleID);
    auto& getParticles()
    {
        return Particles;
    }
    forceGenerator & getGenerator(gID generatorID);
    void removeGenerator(gID generatorID);

    pID addParticle(Particle && toAdd);
    gID addGenerator(std::unique_ptr<forceGenerator> &toAdd);

    template<typename T>
    gID addGenerator(T && toAdd)
    {return addGenerator(std::make_unique<T>(toAdd));}
    void createLinkParticle(pID particleID, gID generatorID);
    void removeLinkParticle(pID particleID, gID generatorID);


    void update(float deltaTime);
};


#endif //RIGIDBOXES_PARTICLEREGISTRY_H