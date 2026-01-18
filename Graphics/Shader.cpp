//
// Created by Clef on 1/13/26.
//

#include "Shader.h"
#include <fstream>
#include <iostream>

#include "../glad/include/glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

void Shader::SetShader(std::string vertexPath, std::string fragmentPath)
{
    std::ifstream ifs (vertexPath);
    std::string vertexContent((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    std::cout << ifs.fail() << "\n";
    ifs.close();
    ifs.open(fragmentPath);
    std::string fragmentContent((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    unsigned int vertex;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    auto vertexCstring = vertexContent.c_str();
    glShaderSource(vertex, 1, &vertexCstring,  nullptr);
    glCompileShader(vertex);

    char buffer[256];
    int success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 256,  nullptr, buffer);
        std::cout << buffer << "\n";
    }


    unsigned int fragment;



    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentCstring = fragmentContent.c_str();
    glShaderSource(fragment, 1, &fragmentCstring, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 256,  nullptr, buffer);
        std::cout << buffer << "\n";
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glDeleteShader(vertex);

    glDeleteShader(fragment);


}

void Shader::use()
{
    glUseProgram(ID);
}
void Shader::setMat4(std::string path, glm::mat4& matrix)
{

    auto pathc = path.c_str();
    glUseProgram(ID);
    glUniformMatrix4fv(glGetUniformLocation(ID, pathc), 1, GL_FALSE, glm::value_ptr(matrix));
}

