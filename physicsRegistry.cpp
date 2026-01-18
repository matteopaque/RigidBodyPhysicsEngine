//
// Created by Clef on 1/15/26.
//

#include "physicsRegistry.h"

#include <forward_list>
#include <ranges>

void physicsRegistry::update(float deltaTime)
{
    erase_if(forceLinks, [&](std::pair<pID,gID> &pair)
    {
        if (Generators.find(pair.second) == Generators.end())
            return true;
        if (Particles.find(pair.first) == Particles.end())
            return true;
        return false;
    });
    for (auto & [particle, generator] : forceLinks)
    {
        Generators[generator]->addForce(Particles[particle]);
    }
    for (auto& particle : Particles | std::views::values)
    {
        particle.integrate(deltaTime);
    }

}

gID physicsRegistry::addGenerator(std::unique_ptr<forceGenerator> &toAdd)
{
    Generators[topGID] = std::move(toAdd);
    const auto id = topGID;
    topGID++;
    return id;
}

pID physicsRegistry::addParticle(Particle&& toAdd)
{
    Particles[topPID] = toAdd;
    const auto id = topPID;
    topPID++;
    return id;
}

void physicsRegistry::createLink(pID particleID, gID generatorID)
{
    forceLinks.emplace_back(particleID, generatorID);
}

forceGenerator& physicsRegistry::getGenerator(gID generatorID)
{
    return *Generators.at(generatorID);
}

Particle& physicsRegistry::getParticle(pID particleID)
{
    return Particles.at(particleID);
}

void physicsRegistry::removeGenerator(gID generatorID)
{
    std::erase_if(forceLinks, [&](std::pair<pID, gID> pair)
    {
        return pair.second==generatorID;
    });
    std::erase_if(Generators, [&](const auto& pair)
    {
       return pair.first == generatorID;
    });
}

void physicsRegistry::removeParticle(pID particleID)
{std::erase_if(forceLinks, [&](std::pair<pID, gID> pair)
    {
        return pair.first == particleID;
    });
    std::erase_if(Particles, [&](const std::pair<pID, Particle>& pair)
    {
       return pair.first == particleID;
    });

}

void physicsRegistry::removeLink(pID particleID, gID generatorID)
{
    std::erase_if(forceLinks, [&](std::pair<pID, gID> pair)
    {
        return pair.first == particleID && pair.second == generatorID;
    });
}


