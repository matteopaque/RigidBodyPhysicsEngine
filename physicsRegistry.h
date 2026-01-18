//
// Created by Clef on 1/15/26.
//

#ifndef RIGIDBOXES_PARTICLEREGISTRY_H
#define RIGIDBOXES_PARTICLEREGISTRY_H
#include "Particle.h"
#include "forceGenerator.h"
#include <map>
#include <memory>

using pID = unsigned int;
using gID = unsigned int;
class physicsRegistry
{
    std::map<pID, Particle> Particles;
    std::map<gID, std::unique_ptr<forceGenerator>> Generators;
    std::vector<std::pair<pID,gID>> forceLinks;
    pID topPID = 0;
    gID topGID = 0;
public:
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
    void createLink(pID particleID, gID generatorID);
    void removeLink(pID particleID, gID generatorID);


    void update(float deltaTime);
};


#endif //RIGIDBOXES_PARTICLEREGISTRY_H