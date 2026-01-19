//
// Created by Clef on 1/15/26.
//

#include "physicsRegistry.h"

#include <forward_list>
#include <ranges>

void physicsRegistry::update(float deltaTime)
{
    erase_if(forceLinksParticle, [&](std::pair<pID,gID> &pair)
    {
        if (Generators.find(pair.second) == Generators.end())
            return true;
        if (Particles.find(pair.first) == Particles.end())
            return true;
        return false;
    });
    erase_if(forceLinksBody, [&](std::pair<rID,gID> &pair)
    {
        if (Generators.find(pair.second) == Generators.end())
            return true;
        if (Bodies.find(pair.first) == Bodies.end())
            return true;
        return false;
    });
    for (auto & [particle, generator] : forceLinksParticle)
    {
        Generators[generator]->addForce(Particles[particle]);
    }
    for (auto & [rigidBody, generator] : forceLinksBody)
    {
        Generators[generator]->addForce(Bodies.at(rigidBody));
    }
    for (auto& particle : Particles | std::views::values)
    {
        particle.integrate(deltaTime);
    }
    for (auto & rigidBody : Bodies | std::views::values)
    {
        rigidBody.integrate(deltaTime);
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

void physicsRegistry::createLinkParticle(pID particleID, gID generatorID)
{
    forceLinksParticle.emplace_back(particleID, generatorID);
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
    std::erase_if(forceLinksParticle, [&](std::pair<pID, gID> pair)
    {
        return pair.second==generatorID;
    });
    std::erase_if(Generators, [&](const auto& pair)
    {
       return pair.first == generatorID;
    });
}

void physicsRegistry::removeParticle(pID particleID)
{std::erase_if(forceLinksParticle, [&](std::pair<pID, gID> pair)
    {
        return pair.first == particleID;
    });
    std::erase_if(Particles, [&](const std::pair<pID, Particle>& pair)
    {
       return pair.first == particleID;
    });

}

void physicsRegistry::removeLinkParticle(pID particleID, gID generatorID)
{
    std::erase_if(forceLinksParticle, [&](std::pair<pID, gID> pair)
    {
        return pair.first == particleID && pair.second == generatorID;
    });
}

RigidBody& physicsRegistry::getRigidBody(rID bodyID)
{
    return Bodies.at(bodyID);
}

rID physicsRegistry::addRigidBody(RigidBody&& body)
{
    Bodies[topRID] = body;
    const auto id = topRID;
    topRID++;
    return id;
}

void physicsRegistry::removeBody(rID toRemove)
{
    erase_if(forceLinksBody, [toRemove](auto & pair)
    {
        return pair.first == toRemove;
    });
    Bodies.erase(toRemove);
}

void physicsRegistry::createLinkBody(rID bodyID, gID generatorID)
{
    forceLinksBody.emplace_back(bodyID, generatorID);
}

void physicsRegistry::removeLinkBody(rID bodyID, gID generatorID)
{
    erase_if(forceLinksBody, [bodyID, generatorID](auto & pair)
    {
        return pair.first == bodyID && pair.second == generatorID;
    });
}
