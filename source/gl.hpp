#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief Shader class for handling shaders.
    class shader {
    private:
        bool init = false;
        uint32_t shaderID = 0;
        uint32_t type = 0;
        char* source = nullptr;
        char* errorBuffer = nullptr;

    public:
        shader() {}
        /// @brief Image object constructor.
        /// @param source The desired shader source code.
        /// @param type The desired shader type.
        shader(const char* source, uint32_t type) {
            init = true;
            shaderID = glCreateShader(type);
            setData(source);
        }
        ~shader() {
            if(init) {
                delete[] source;
                delete[] errorBuffer;
                glDeleteShader(shaderID);
            }
        }

        /// @brief Changes the shader source code, call "compile()" after this to push changes onto GPU.
        /// @param source The desired shader source code.
        void setData(const char* source) {
            init = true;
            this->source = new char[strlen(source)];
            strcpy(this->source, source);
            glShaderSource(shaderID, 1, &source, NULL);
        }

        /// @brief Gets the shader ID.
        /// @return Shader ID.
        uint32_t getID() {
            return shaderID;
        }
        /// @brief Gets the shader type.
        /// @return Shader type.
        uint32_t getType() {
            return type;
        }
        /// @brief Gets the shader source.
        /// @return The shader source code (DO NOT MODIFY).
        char* getSource() {
            return source;
        }
        /// @brief Gets the shader compile error.
        /// @param errorBufferSize THe desired error buffer size.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError(size_t errorBufferSize = 512) {
            errorBuffer = new char[errorBufferSize];
            glGetShaderInfoLog(shaderID, errorBufferSize, NULL, errorBuffer);
            return errorBuffer;
        }

        /// @brief Compiles the shader, call this after "setData()" to push changes to GPU.
        void compile() {
            glCompileShader(shaderID);
        }
        /// @brief Checks if the shader compiled successfuly.
        /// @return True if the shader compiled successfuly, and false if otherwise, check "getError()" for more info.
        bool valid() {
            int success = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            return success;
        }
    };

    /// @brief Program class for using shaders, compatible with "PNT::shader" and shader identifier of type "uint32_t" to be linked in.
    class program {
    private:
        size_t attachedCount = 0;
        uint32_t programID = 0;
        char* errorBuffer = nullptr;

    public:
        program() {}
        /// @brief Program object constructor for handling shaders.
        /// @param shaders Shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        program(std::initializer_list<PNT::shader*> shaders) {
            attachedCount = shaders.size();
            programID = glCreateProgram();
            for(size_t i = 0; i < shaders.size(); i++) {
                glAttachShader(programID, shaders.begin()[i]->getID());
            }
        }
        /// @brief Program object constructor for handling shaders.
        /// @param shaders Shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        program(std::initializer_list<uint32_t> shaders) {
            attachedCount = shaders.size();
            programID = glCreateProgram();
            for(size_t i = 0; i < shaders.size(); i++) {
                glAttachShader(programID, shaders.begin()[i]);
            }
        }
        ~program() {
            if(attachedCount) {
                delete[] errorBuffer;
                glDeleteProgram(programID);
            }
        }

        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(shader* object) {
            attachedCount++;
            glAttachShader(programID, object->getID());
        }
        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(uint32_t object) {
            attachedCount++;
            glAttachShader(programID, object);
        }
        /// @brief Detaches the connected shader from the program.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(shader* object) {
            attachedCount--;
            glDetachShader(programID, object->getID());
        }
        /// @brief Detaches the connected shader from the program.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(uint32_t object) {
            attachedCount--;
            glDetachShader(programID, object);
        }
        void use() {
            glUseProgram(programID);
        }

        /// @brief Gets the program compile error.
        /// @param errorBufferSize The desired error buffer size.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError(size_t errorBufferSize = 512) {
            errorBuffer = new char[errorBufferSize];
            glGetProgramInfoLog(programID, errorBufferSize, NULL, errorBuffer);
            return errorBuffer;
        }

        /// @brief Links the program, call this after "attachShader()" to push the changes onto the GPU.
        void link() {
            glLinkProgram(programID);
        }
        /// @brief Checks if the program linked successfuly.
        /// @return True if the program linked successfuly, and false if otherwise, check "getError()" for more info.
        bool valid() {
            int success = 0;
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            return success;
        }
    };
}
