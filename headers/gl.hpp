#pragma once

#include <stdint.h>
#include <string>
#include <initializer_list>

struct GladGLContext;

namespace PNT {
    /// @brief Shader class for handling shaders.
    class shader {
    private:
        GladGLContext* m_openglContext;
        uint32_t m_shaderID;
        uint32_t m_type;
        char* m_source;
        char m_errorBuffer[1024];
        int m_success;

    public:
        /// @brief Shader object constructor.
        /// @param openglContext The desired OpenGL context to put the shader in.
        shader(GladGLContext* openglContext);

        /// @brief Shader object constructor.
        /// @param source The desired shader source code.
        /// @param type The desired type of shader to create.
        /// @param openglContext The desired OpenGL context to put the shader in.
        shader(const char* source, uint32_t type, GladGLContext* openglContext);

        ~shader();

        /// @brief Creates the shader.
        /// @param type The desired type of shader to create.
        void createShader(uint32_t type);

        /// @brief Destroys the shader.
        void destroyShader();

        /// @brief Changes the shader source code, call "compile()" after this to push changes onto GPU.
        /// @param source The desired shader source code (must be null terminated).
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
        /// @return The error buffer.
        std::string getError();

        /// @brief Compiles the shader, call this after "setData()" to push changes to GPU.
        void compile();

        /// @brief Checks if the shader compiled successfuly.
        /// @return True if the shader compiled successfuly, and false if otherwise, check "getError()" for more info.
        bool valid();
    };

    /// @brief Program class for using shaders, compatible with "PNT::shader" and shader identifier of type "uint32_t" to be linked in.
    class program {
    private:
        GladGLContext* m_openglContext;
        uint32_t programID;
        char errorBuffer[1024];
        int success;

    public:
        /// @brief Program object constructor for handling shaders.
        /// @param openglContext The desired OpenGL context to put the program in.
        program(GladGLContext* openglContext);

        /// @brief Program object constructor for handling shaders.
        /// @param shaders The desired shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        /// @param openglContext The desired OpenGL context to put the program in.
        program(std::initializer_list<PNT::shader*> shaders, GladGLContext* openglContext);

        /// @brief Program object constructor for handling shaders.
        /// @param shaders The desired shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        /// @param openglContext The desired OpenGL context to put the program in.
        program(std::initializer_list<uint32_t> shaders, GladGLContext* openglContext);

        ~program();

        /// @brief Create the program.
        /// @param shaders The desired shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        void createProgram(std::initializer_list<PNT::shader*> shaders);

        /// @brief Create the program.
        /// @param shaders The desired shaders to link into the program, they can be ether "PNT::shader*" or a shader identifier of type "uint32_t" contained in curly brackets (only one type at a time).
        void createProgram(std::initializer_list<uint32_t> shaders);

        void destroyProgram();

        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object The desired shader object to link, can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(shader* object);

        /// @brief Links a shader to the program, call "link()" after this to relink the program and push changes onto the GPU.
        /// @param object The desired shader object to link, can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void attachShader(uint32_t object);

        /// @brief Detaches the connected shader from the program.
        /// @param object The desired shader object to link, can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(shader* object);

        /// @brief Detaches the connected shader from the program.
        /// @param object The desired shader object to link, can be a "PNT::shader*" or a shader identifier of type "uint32_t".
        void detachShader(uint32_t object);

        /// @brief Links the program, call this after "attachShader()" to push the changes onto the GPU.
        void link();

        /// @brief Use the shader.
        void use();

        uint32_t getID();

        /// @brief Gets the program compile error.
        /// @return The error buffer (DO NOT MODIFY).
        const char* getError();

        /// @brief Checks if the program linked successfuly.
        /// @return True if the program linked successfuly, and false if otherwise, check "getError()" for more info.
        bool valid();
    };
}
