#ifndef SHADER_H
#define SHADER_H

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
using namespace std;

class Shader
{
public:
    unsigned int ID; // 程序ID

    // 构造器读取并构建着色器
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    // 使用/激活程序
    void use();
    unsigned int CreateShader(int type, const char *code);
    // uniform工具函数
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    void setVec3(const string &name, float x, float y, float z) const;
    void setVec4(const string &name, const glm::vec4 &value) const;
    void setMat4(const string &name, const glm::mat4 &value) const;
};

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{

    // 1. 从文件路径中获取顶点/片段着色器
    string vertexCode, fragmentCode;
    ifstream vShaderFile, fShaderFile;

    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "shader file read:" << e.what() << '\n';
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. 编译着色器
    unsigned int vertex = CreateShader(GL_VERTEX_SHADER, vShaderCode);
    unsigned int fragment = CreateShader(GL_FRAGMENT_SHADER, fShaderCode);
    int success;
    char infoLog[512];

    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
        cout << "shader program error:\n"
             << infoLog << endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::CreateShader(int type, const char *code)
{
    unsigned int shaderID;
    int success;
    char infoLog[512];

    // 顶点着色器
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
        cout << "complie shader error:\n"
             << infoLog << endl;
    }
    return shaderID;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

#endif // SHADER_H