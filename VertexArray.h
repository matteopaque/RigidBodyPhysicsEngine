//
// Created by Clef on 1/13/26.
//

#ifndef RIGIDBOXES_VERTEXARRAY_H
#define RIGIDBOXES_VERTEXARRAY_H
#include <vector>


class VertexArray
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    bool indexed;
    int size;
    int Esize;
public:

    void InitLoadData(float * data, int size);
    void InitLoadData(float * data, int dataSize, unsigned int * EBOdata, int EBOsize);
    void bind();
    void draw();
};


#endif //RIGIDBOXES_VERTEXARRAY_H