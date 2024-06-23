#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief Shader class for handling shaders.
    class shader {
    private:
        bool init = false;
        unsigned int shaderID = 0;
        unsigned int type = 0;
        char* source = nullptr;
        char* errorBuffer = nullptr;

    public:
        shader(const char* source, unsigned int type) {
            init = true;
            shaderID = glCreateShader(type);
            setData(source);
        }
        shader() {}
        ~shader() {
            if(init) {
                delete[] source;
                delete[] errorBuffer;
            }
            glDeleteShader(shaderID);
        }

        /// @brief Gets the shader ID.
        /// @return Shader ID.
        unsigned int getID() {
            return shaderID;
        }
        /// @brief Gets the shader type.
        /// @return Shader type.
        unsigned int getType() {
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

        /// @brief Compiles the shader, call this after "setData()", to push changes to GPU.
        void compile() {
            glCompileShader(shaderID);
        }
        /// @brief Checks if the shader compiled successfuly.
        /// @return True if the shader compiled successfuly, and false if there was an error, check "getError()" for more info.
        bool valid() {
            int success = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            return success;
        }
        /// @brief Changes the shader source code, call "compile()" after this to push changes to GPU.
        /// @param source The desired shader source code.
        void setData(const char* source) {
            this->source = new char[strlen(source)];
            strcpy(this->source, source);
            glShaderSource(shaderID, 1, &source, NULL);
        }
    };
}
