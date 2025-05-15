
#include "my_shader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

MyShader::MyShader(const char* vtxPath, const char* frgPath) {
    std::string vtxCode;
    std::string frgCode;
    std::ifstream vShaderFile;  // vertex
    std::ifstream fShaderFile;  // fragment

    // 保证ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vtxPath);
        fShaderFile.open(frgPath);
        std::stringstream vss, fss; // shader stream
        vss<<vShaderFile.rdbuf();
        fss<<fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vtxCode = vss.str();
        frgCode = fss.str();
    } catch (std::ifstream::failure e) {
        std::cout<<"ERROR: MyShader: read file failed."<<std::endl;
    }
    
    const char* vShaderCode = vtxCode.c_str();
    const char* fShaderCode = frgCode.c_str();

    // compile, link shader
    unsigned int vtx, frg;
    int ok;
    char info[512];

    // vertex shader
    vtx = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx, 1, &vShaderCode, NULL);
    glCompileShader(vtx);

    glGetShaderiv(vtx, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vtx, 512, NULL, info);
        std::cout<<"ERROR: vertex shader: compile failed\n"<<info<<std::endl;
    }

    // fragment shader
    frg = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg, 1, &fShaderCode, NULL);
    glCompileShader(frg);

    glGetShaderiv(frg, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(frg, 512, NULL, info);
        std::cout<<"ERROR: fragment shader: compile failed\n"<<info<<std::endl;
    }

    // shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, vtx);
    glAttachShader(id, frg);
    glLinkProgram(id);

    glGetShaderiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(id, 512, NULL, info);
        std::cout<<"ERROR: shader program: link failed\n"<<info<<std::endl;
    }
    glDeleteShader(vtx);
    glDeleteShader(frg);
}

void MyShader::use() {
    glUseProgram(this->id);
}

void MyShader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void MyShader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void MyShader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

unsigned int MyShader::getId() {
    return this->id;
}

