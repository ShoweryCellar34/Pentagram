#include <gl.hpp>

#include <cstring>
#include <glad.h>

namespace PNT {
    // Shader definitions.

    shader::shader() {
        errorBuffer[0] = 0;
    }

    shader::shader(const char* source, uint32_t type) {
        errorBuffer[0] = 0;
        shaderID = glCreateShader(type);
        setData(source);
    }

    shader::~shader() {
        delete[] source;
        if(shaderID) {
            glDeleteShader(shaderID);
        }
    }

    void shader::setData(const char* source) {
        this->source = new char[strlen(source)];
        strcpy(this->source, source);
        glShaderSource(shaderID, 1, &source, NULL);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, errorBuffer);
        }
    }

    uint32_t shader::getID() {
        return shaderID;
    }

    uint32_t shader::getType() {
        return type;
    }

    char* shader::getSource() {
        return source;
    }

    const char* shader::getError() {
        return errorBuffer;
    }

    void shader::compile() {
        glCompileShader(shaderID);
    }

    bool shader::valid() {
        return success;
    }

    // Program definitions.

    program::program() {
        errorBuffer[0] = 0;
    }

    program::program(std::initializer_list<PNT::shader*> shaders) {
        errorBuffer[0] = 0;
        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            glAttachShader(programID, shaders.begin()[i]->getID());
        }
    }

    program::program(std::initializer_list<uint32_t> shaders) {
        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            glAttachShader(programID, shaders.begin()[i]);
        }
    }

    void program::attachShader(shader* object) {
        glAttachShader(programID, object->getID());
    }

    void program::attachShader(uint32_t object) {
        glAttachShader(programID, object);
    }

    void program::detachShader(shader* object) {
        glDetachShader(programID, object->getID());
    }

    void program::detachShader(uint32_t object) {
        glDetachShader(programID, object);
    }

    void program::link() {
        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(programID, 1024, NULL, errorBuffer);
        }
    }

    void program::use() {
        glUseProgram(programID);
    }

    uint32_t program::getID() {
        return programID;
    }

    const char* program::getError() {
        return errorBuffer;
    }

    bool program::valid() {
        return success;
    }
}
