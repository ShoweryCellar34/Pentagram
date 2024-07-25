#include <gl.hpp>

#include <memory>
#include <cstring>
#include <spdlog/spdlog.h>
#include <glad.h>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Shader definitions.

    shader::shader() {
    }

    shader::shader(const char* source, uint32_t type) {
        errorBuffer[0] = 0;
        createShader(type);
        setData(source);
    }

    shader::~shader() {
        if(source) {
            delete[] source;
        }
        destroyShader();
    }

    void shader::createShader(uint32_t type) {
        shaderID = glCreateShader(type);
    }

    void shader::destroyShader() {
        if(shaderID) {
            glDeleteShader(shaderID);
            shaderID = 0;
        }
    }

    void shader::setData(const char* source) {
        logger.get()->info("[PNT]Setting data for shader with ID: {}", shaderID);

        this->source = new char[strlen(source)];
        strcpy(this->source, source);
        glShaderSource(shaderID, 1, &source, NULL);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, errorBuffer);
            logger.get()->warn("[PNT]Failed to set shader data with error \"{}\"", errorBuffer);
        } else {
            success = 1;
            for(int i = 0; i < 1024; i++) {
                errorBuffer[i] = 0;
            }
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
        logger.get()->info("[PNT]Attatching shader to program with ID: {}", programID);

        if(!object->getID()) {
            glAttachShader(programID, object->getID());
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 is invaild)");
        }
    }

    void program::attachShader(uint32_t object) {
        logger.get()->info("[PNT]Attatching shader to program with ID: {}", programID);

        if(!object) {
            glAttachShader(programID, object);
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 is invaild)");
        }
    }

    void program::detachShader(shader* object) {
        glDetachShader(programID, object->getID());
    }

    void program::detachShader(uint32_t object) {
        glDetachShader(programID, object);
    }

    void program::link() {
        logger.get()->info("[PNT]Linking program with ID: {}", programID);

        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(programID, 1024, NULL, errorBuffer);
            logger.get()->warn("[PNT]Failed to link program with error \"{}\"", errorBuffer);
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
