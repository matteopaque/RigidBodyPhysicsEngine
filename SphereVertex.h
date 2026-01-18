//
// Created by Clef on 12/29/25.
//

#ifndef OPENGLSETUP_SPHERE_H
#define OPENGLSETUP_SPHERE_H
#include <vector>


class SphereVertex
{
public:
    void buildVertices(int sectorCount, int stackCount);
    float* getVertices();
    int getVerticesSize();
    unsigned int* getIndices();
    int getIndicesSize();

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};


#endif //OPENGLSETUP_SPHERE_H
