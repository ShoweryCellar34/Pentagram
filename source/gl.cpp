#include <gl.hpp>

#include <memory>
#include <cstring>
#include <spdlog/spdlog.h>
#include <glad/gl.h>
#include <error.hpp>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Shader definitions.

    shader::shader(GladGLContext* openglContext) : m_openglContext(openglContext), m_shaderID(0), m_type(0), m_source(nullptr), m_errorBuffer{0}, m_success(0) {
    }

    shader::shader(const char* source, uint32_t type, GladGLContext* openglContext) : m_openglContext(openglContext), m_shaderID(0), m_type(0), m_source(nullptr), m_errorBuffer{0}, m_success(0) {
        createShader(type);
        setData(source);
    }

    shader::~shader() {
        delete[] m_source;
        destroyShader();
    }

    void shader::createShader(uint32_t type) {
        PNT_NO_SHADER_ID(m_shaderID);

        logger.get()->info("[PNT]Creating shader");

        m_shaderID = m_openglContext->CreateShader(type);
    }

    void shader::destroyShader() {
        PNT_SHADER_ID(m_shaderID);

        logger.get()->info("[PNT]Destroying shader");

        m_openglContext->DeleteShader(m_shaderID);
        m_shaderID = 0;
    }

    void shader::setData(const char* source) {
        PNT_SHADER_ID(m_shaderID);

        logger.get()->debug("[PNT]Setting data for shader with ID: {}", m_shaderID);

        if(this->m_source != nullptr) {
            delete[] this->m_source;
        }
        if(strlen(source) > 0) {
            this->m_source = new char[strlen(source) + 1];
            strcpy(this->m_source, source);
        } else {
            this->m_source = new char[1];
            this->m_source[0] = 0;
        }
        m_openglContext->ShaderSource(m_shaderID, 1, &source, NULL);
        m_openglContext->GetShaderiv(m_shaderID, GL_COMPILE_STATUS, &m_success);
        if(m_success) {
            m_openglContext->GetShaderInfoLog(m_shaderID, 1024, NULL, m_errorBuffer);
            logger.get()->warn("[PNT]Failed to set shader data with error \"{}\"", m_errorBuffer);
        } else {
            m_success = 1;
            for(int i = 0; i < 1024; i++) {
                m_errorBuffer[i] = 0;
            }
        }
    }

    uint32_t shader::getID() {
        return m_shaderID;
    }

    uint32_t shader::getType() {
        return m_type;
    }

    char* shader::getSource() {
        return m_source;
    }

    std::string shader::getError() {
        return (std::string)m_errorBuffer;
    }

    void shader::compile() {
        m_openglContext->CompileShader(m_shaderID);
    }

    bool shader::valid() {
        return m_success;
    }

    // Program definitions.

    program::program(GladGLContext* openglContext) : m_openglContext(openglContext), programID(0), errorBuffer{0}, success(0) {
    }

    program::program(std::initializer_list<PNT::shader*> shaders, GladGLContext* openglContext) : m_openglContext(openglContext), programID(0), errorBuffer{0}, success(0) {
        createProgram(shaders);
    }

    program::program(std::initializer_list<uint32_t> shaders, GladGLContext* openglContext) : m_openglContext(openglContext), programID(0), errorBuffer{0}, success(0) {
        createProgram(shaders);
    }

    program::~program() {
        destroyProgram();
    }

    void program::createProgram(std::initializer_list<PNT::shader*> shaders) {
        PNT_NO_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Creating program");

        programID = m_openglContext->CreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            attachShader(shaders.begin()[i]->getID());
        }
    }

    void program::createProgram(std::initializer_list<uint32_t> shaders) {
        PNT_NO_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Creating program");

        programID = m_openglContext->CreateProgram();
        for(size_t i = 0; i < shaders.size(); i++) {
            attachShader(shaders.begin()[i]);
        }
    }

    void program::destroyProgram() {
        PNT_PROGRAM_ID(programID);

        logger.get()->info("[PNT]Destroying program");

        m_openglContext->DeleteProgram(programID);
        programID = 0;
    }

    void program::attachShader(shader* object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Attatching shader with ID: {} to program with ID: {}", object->getID(), programID);

        if(object->getID()) {
            m_openglContext->AttachShader(programID, object->getID());
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 indicates not created yet)");
        }
    }

    void program::attachShader(uint32_t object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Attatching shader with ID: {} to program with ID: {}", object, programID);

        if(object) {
            m_openglContext->AttachShader(programID, object);
        } else {
            logger.get()->warn("[PNT]Attempted to attach shader with ID 0 (0 indicates not created yet)");
        }
    }

    void program::detachShader(shader* object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Detching shader with ID: {} to program with ID: {}", object->getID(), programID);

        m_openglContext->DetachShader(programID, object->getID());
    }

    void program::detachShader(uint32_t object) {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Detching shader with ID: {} to program with ID: {}", object, programID);

        m_openglContext->DetachShader(programID, object);
    }

    void program::link() {
        PNT_PROGRAM_ID(programID);

        logger.get()->debug("[PNT]Linking program with ID: {}", programID);

        m_openglContext->LinkProgram(programID);
        m_openglContext->GetProgramiv(programID, GL_LINK_STATUS, &success);
        if(!success) {
            m_openglContext->GetProgramInfoLog(programID, 1024, NULL, errorBuffer);
            logger.get()->warn("[PNT]Failed to link program with error \"{}\"", errorBuffer);
        }
    }

    void program::use() {
        PNT_PROGRAM_ID(programID);

        m_openglContext->UseProgram(programID);
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
