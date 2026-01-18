//
// Created by Clef on 1/13/26.
//

#include "VertexArray.h"


#include "../glad/include/glad/glad.h"

// probably not needed
void VertexArray::bind()
{
    glBindVertexArray(VAO);
}
// size in bytes
void VertexArray::InitLoadData(float* data, int dataSize)
{
    size = dataSize;
    VAO = 0;
    VBO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    indexed = false;
}

void VertexArray::InitLoadData(float* data, int dataSize, unsigned int* EBOdata, int EBOsize)
{
    Esize = EBOsize;
    InitLoadData(data, dataSize);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOsize, EBOdata, GL_STATIC_DRAW);
    indexed = true;
}


// auto binds the vertex array
void VertexArray::draw()
{
    glBindVertexArray(VAO);
    if (!indexed)
    {
        glDrawArrays(GL_TRIANGLES, 0, size/sizeof(float));
        return;
    }
    glDrawElements(GL_TRIANGLES, Esize/sizeof(unsigned int), GL_UNSIGNED_INT, 0 );
}



