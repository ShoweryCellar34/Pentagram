#include <gl.hpp>

#include <memory>
#include <cstring>
#include <spdlog/spdlog.h>
#include <glad.h>
#include <error.hpp>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Shader definitions.

    shader::shader() : shaderID(0), type(0), source(0), errorBuffer{0}, success(0) {
    }

    shader::shader(const char* source, uint32_t type) : shaderID(0), type(0), source(0), errorBuffer{0}, success(0) {
        errorBuffer[0] = 0;
        createShader(type);
        setData(source);
    }

    shader::~shader() {
        delete[] source;
        destroyShader();
    }

    void shader::createShader(uint32_t type) {
        PNT_NO_SHADER_ID(shaderID);

        logger.get()->info("[PNT]Creating shader");

        shaderID = glCreateShader(type);
    }

    void shader::destroyShader() {
        PNT_SHADER_ID(shaderID);

        logger.get()->info("[PNT]Destroying shader");

        glDeleteShader(shaderID);
        shaderID = 0;
    }

    void shader::setData(const char* source) {
        PNT_SHADER_ID(shaderID);

        logger.get()->debug("[PNT]Setting data for shader with ID: {}", shaderID);

        if(strlen(source) > 0) {
            this->source = new char[strlen(source)];
            strcpy(this->source, source);
        } else {
            this->source = new char[1];
            this->source[0] = 0;
        }
        glShaderSource(shaderID, 1, &source, NULL);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(success) {
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

    program::program() : programID(0), errorBuffer{0}, success(0) {
    }

    program::program(std::initializer_list<PNT::shader*> shaders) : programID(0), errorBuffer{0}, success(0) {
        createProgram(shaders);
    }

    program::program(std::initializer_list<uint32_t> shaders) : programID(0), errorBuffer{0}, success(0) {
        createProgram(shaders);
    }

    program::~program() {
        destroyProgram();
    }

    void program::createProgram(std::initializer_list<PNT::shader*> shaders) {
        PNT_NO_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Creating program");

        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            attachShader(shaders.begin()[i]->getID());
        }
    }

    void program::createProgram(std::initializer_list<uint32_t> shaders) {
        PNT_NO_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Creating program");

        programID = glCreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            attachShader(shaders.begin()[i]);
        }
    }

    void program::destroyProgram() {
        PNT_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Destroying program");

        glDeleteProgram(programID);
        programID = 0;
    }

    void program::attachShader(shader* object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Attatching shader with ID: {} to program with ID: {}", object->getID(), programID);

        if(object->getID()) {
            glAttachShader(programID, object->getID());
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 indicates not created yet)");
        }
    }

    void program::attachShader(uint32_t object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Attatching shader with ID: {} to program with ID: {}", object, programID);

        if(object) {
            glAttachShader(programID, object);
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 indicates not created yet)");
        }
    }

    void program::detachShader(shader* object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Detching shader with ID: {} to program with ID: {}", object->getID(), programID);

        glDetachShader(programID, object->getID());
    }

    void program::detachShader(uint32_t object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Detching shader with ID: {} to program with ID: {}", object, programID);

        glDetachShader(programID, object);
    }

    void program::link() {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Linking program with ID: {}", programID);

        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(programID, 1024, NULL, errorBuffer);
            logger.get()->warn("[PNT]Failed to link program with error \"{}\"", errorBuffer);
        }
    }

    void program::use() {
        PNT_PROGRAM_ID(programID);

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
