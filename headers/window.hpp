#pragma once

#include <cstdint>
#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <image.hpp>
#include <event.hpp>

namespace PNT {
    class Window;

    struct windowData {
        void(*eventCallback)(Window*, windowEvent) = nullptr;
        std::string title;
        image icon;
        uint32_t width = 0, height = 0;
        uint32_t xpos = 0, ypos = 0;
        bool hidden = false;
        bool iconified = false;
        uint8_t vsyncMode = 0;
        float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    class callbackManagers {
    private:
        friend class Window;

        static void keyCallbackManager(GLFWwindow*, int, int, int, int);
        static void charCallbackManager(GLFWwindow*, unsigned int);
        static void dropCallbackManager(GLFWwindow*, int, const char**);
        static void scrollCallbackManager(GLFWwindow*, double, double);
        static void cursorPosCallbackManager(GLFWwindow*, double, double);
        static void windowposCallbackManager(GLFWwindow*, int, int);
        static void windowsizeCallbackManager(GLFWwindow*, int, int);
        static void mousebuttonCallbackManager(GLFWwindow*, int, int, int);
        static void iconifyCallbackManager(GLFWwindow*, int);
    };

    class Window {
    private:
        friend class callbackManagers;
        friend void deinit();

    public:
        /// @brief Winfow object constructor.
        /// @param title The desired title.
        /// @param width The desired width.
        /// @param height The desired height.
        /// @param xpos The desired x position.
        /// @param ypos The desired y position.
        /// @param ImGuiFlags The desired imgui gui flags.
        Window(const char* title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags);

        ~Window();

        /// @brief Starts the opengl and imgui frame for the window.
        void startFrame();

        /// @brief Hides the window, returns the sdl error code (0 is success).
        void endFrame();

        /// @brief Sets the event callback of the window that will be call every time there is an event.
        /// @param newEventCallback The desired function pointer for the event callback with signature "PNT::Window*, PNT::windowEvent" (use nullptr to clear callback).
        void setEventCallback(void(*newEventCallback)(Window*, windowEvent));

        /// @brief Check if the currect window should close.
        /// @return True if the window should close.
        bool shouldClose();

        /// @brief Sets the data struct of the window.
        /// @param newData The desired PNT::windowData for the window.
        void setWindowData(windowData newData);

        /// @brief Sets the window title.
        /// @param title The desired window title.
        void setTitle(const char* title);

        /// @brief Sets the window icon.
        /// @param image The desired PNT::image for the window icon.
        /// @return Will return false if the image is invalid, this happens if the contents of the image are null.
        bool setIcon(const image& image);

        /// @brief Sets the dimentions of the window.
        /// @param width The desired window width.
        /// @param height The desired window height.
        void setDimentions(uint16_t width, uint16_t height);

        /// @brief Sets the position of the window.
        /// @param xpos The desired x window position.
        /// @param ypos The desired y window position.
        void setPosition(uint16_t xpos, uint16_t ypos);

        /// @brief Hides the window.
        void hide();

        /// @brief Shows the window.
        void show();

        /// @brief Minimises the window.
        void minimize();

        /// @brief Maximises the window.
        void maximize();

        /// @brief Sets the vsync mode for the window.
        /// @param vsyncMode The desired vsync mode for the window, 1 = on, 0 = off, -1 adaptive.
        void setVsyncMode(uint8_t vsyncMode);

        /// @brief Sets the opengl clear color for the window.
        /// @param red The desired red channel.
        /// @param green The desired green channel.
        /// @param blue The desired blue channel.
        /// @param alpha The desired alpha channel.
        void setClearColor(float red, float green, float blue, float alpha);

        /// @brief Get the window data.
        /// @return A PNT::windowData struct.
        windowData getWindowData();

    private:
        static inline size_t instances;
        GLFWwindow* window;
        windowData data;
        ImGuiContext* ImGuiContext;
        ImGuiIO* IO;
    };
}
