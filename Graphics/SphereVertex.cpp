//
// Created by Clef on 12/29/25.
//

#include "SphereVertex.h"
#include <numbers>
#include <cmath>
#include <iostream>

void SphereVertex::buildVertices(int sectorCount, int stackCount)
{
    float pi = std::numbers::pi;
    vertices.clear();
    indices.clear();
    for (int i = 0; i < stackCount; i++)
    {
        float phi = (pi / 2.f) - pi * ((float)i / (float)(stackCount - 1));
        float y = std::sin(phi);
        for (int j = 0; j < sectorCount; j++)
        {
            float theta = 2 * pi * ((float)j / (float)sectorCount);
            float x = std::cos(theta) * std::cos(phi);
            float z = std::sin(theta) * std::cos(phi);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    int k1, k2;
    for (int i = 0; i < stackCount - 1; ++i)
    {
        k1 = i * sectorCount;
        k2 = k1 + sectorCount;
        for (int j = 0; j < sectorCount; j++)
        {
            int nextK1 = (j == sectorCount - 1) ? k1 : k1 + 1 + j;
            int nextK2 = (j == sectorCount - 1) ? k2 : k2 + 1 + j;
            if (i != 0)
            {
                indices.push_back(k1 + j);
                indices.push_back(k2 + j);
                indices.push_back(nextK1);
            }
            if (i != (stackCount - 2))
            {
                indices.push_back(nextK1);
                indices.push_back(k2 + j);
                indices.push_back(nextK2);
            }
        }
    }
    for (int i = 0; i < indices.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
        }
        if ((i + 1) % 3 == 0)
        {
        }
    }
}

float* SphereVertex::getVertices()
{
    return vertices.data();
}

int SphereVertex::getVerticesSize()
{
    return vertices.size();
}

unsigned int* SphereVertex::getIndices()
{
    return indices.data();
}

int SphereVertex::getIndicesSize()
{
    return indices.size();
}

