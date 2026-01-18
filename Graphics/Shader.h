//
// Created by Clef on 1/13/26.
//

#ifndef RIGIDBOXES_SHADER_H
#define RIGIDBOXES_SHADER_H
#include <string>
#include <glm/glm.hpp>

class Shader
{

public:
    unsigned int ID;
    Shader(){}
    void SetShader(std::string vertexPath, std::string fragmentPath);
    void setMat4(std::string path, glm::mat4& matrix);
    void use();
};


#endif //RIGIDBOXES_SHADER_H