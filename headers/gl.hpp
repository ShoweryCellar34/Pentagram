#pragma once

#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace PNT {
    /// @brief Shader class for handling shaders.
    class shader {
    private:
        uint32_t shaderID = 0;
        uint32_t type = 0;
        char* source = new char[1];
        char* errorBuffer = new char[1];

    public:
        shader();

        /// @brief Image object constructor.
        /// @param source The desired shader source code.
        /// @param type The desired shader type.
        shader(const char* source, uint32_t type);

        ~shader();

        /// @brief Changes the shader source code, call "compile()" after this to push changes onto GPU.
        /// @param source The desired shader source code.
        void setData(const char* source);

        /// @brief Gets the shader ID.
        /// @return Shader ID.
        uint32_t getID();

        /// @brief Gets the shader type.
        /// @return Shader type.
        uint32_t getType();

        /// @brief Gets the shader source.
        /// @return The shader source code (DO NOT MODIFY).
        char* getSource();

        /// @brief Gets the shader compile error.
        /// @param errorBufferSize THe desired error buffer size.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError(size_t errorBufferSize);

        /// @brief Compiles the shader, call this after "setData()" to push changes to GPU.
        void compile();

        /// @brief Checks if the shader compiled successfuly.
        /// @return True if the shader compiled successfuly, and false if otherwise, check "getError()" for more info.
        bool valid();
    };

    /// @brief Program class for using shaders, compatible with "PNT::shader" and shader identifier of type "uint32_t" to be linked in.
    class program {
    private:
        size_t attachedCount = 0;
        uint32_t programID = 0;
        char* errorBuffer = nullptr;

    public:
        program();

        /// @brief Program object constructor for handling shaders.
        /// @param shaders Shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        program(std::initializer_list<PNT::shader*> shaders);

        /// @brief Program object constructor for handling shaders.
        /// @param shaders Shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        program(std::initializer_list<uint32_t> shaders);

        ~program();

        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(shader* object);

        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(uint32_t object);

        /// @brief Detaches the connected shader from the program.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(shader* object);

        /// @brief Detaches the connected shader from the program.
        /// @param object Can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(uint32_t object);

        void use();

        /// @brief Gets the program compile error.
        /// @param errorBufferSize The desired error buffer size.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError(size_t errorBufferSize);

        /// @brief Links the program, call this after "attachShader()" to push the changes onto the GPU.
        void link();

        /// @brief Checks if the program linked successfuly.
        /// @return True if the program linked successfuly, and false if otherwise, check "getError()" for more info.
        bool valid();
    };
}
