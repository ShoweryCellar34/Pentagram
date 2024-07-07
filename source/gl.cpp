#include <gl.hpp>

#include <cstring>
#include <glad.h>

namespace PNT {
    // Shader definitions.

    shader::shader() = default;

    shader::shader(const char* source, uint32_t type) {
        shaderID = glCreateShader(type);
        setData(source);
    }

    shader::~shader() {
        delete[] source;
        delete[] errorBuffer;
        if(shaderID) {
            glDeleteShader(shaderID);
        }
    }

    void shader::setData(const char* source) {
        this->source = new char[strlen(source)];
        strcpy(this->source, source);
        glShaderSource(shaderID, 1, &source, NULL);
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

    const char* shader::getError(size_t errorBufferSize = 512) {
        errorBuffer = new char[errorBufferSize];
        glGetShaderInfoLog(shaderID, errorBufferSize, NULL, errorBuffer);
        return errorBuffer;
    }

    void shader::compile() {
        glCompileShader(shaderID);
    }

    bool shader::valid() {
        int success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        return success;
    }

    // Program definitions.

    program::program() = default;

    program::program(std::initializer_list<PNT::shader*> shaders) {
        attachedCount = shaders.size();
        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            glAttachShader(programID, shaders.begin()[i]->getID());
        }
    }

    program::program(std::initializer_list<uint32_t> shaders) {
        attachedCount = shaders.size();
        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            glAttachShader(programID, shaders.begin()[i]);
        }
    }

    program::~program() {
        if(attachedCount) {
            delete[] errorBuffer;
            glDeleteProgram(programID);
        }
    }

    void program::attachShader(shader* object) {
        attachedCount++;
        glAttachShader(programID, object->getID());
    }

    void program::attachShader(uint32_t object) {
        attachedCount++;
        glAttachShader(programID, object);
    }

    void program::detachShader(shader* object) {
        attachedCount--;
        glDetachShader(programID, object->getID());
    }

    void program::detachShader(uint32_t object) {
        attachedCount--;
        glDetachShader(programID, object);
    }

    void program::use() {
        glUseProgram(programID);
    }

    const char* program::getError(size_t errorBufferSize = 512) {
        errorBuffer = new char[errorBufferSize];
        glGetProgramInfoLog(programID, errorBufferSize, NULL, errorBuffer);
        return errorBuffer;
    }

    uint32_t program::getID() {
        return programID;
    }

    void program::link() {
        glLinkProgram(programID);
    }

    bool program::valid() {
        int success = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        return success;
    }
}
