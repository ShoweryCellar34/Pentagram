#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <image.hpp>

class GLFWwindow;
struct ImGuiContext;
struct ImGuiIO;

namespace PNT {
    class Window;
    struct windowEvent;

    struct windowData {
        void(*eventCallback)(Window*, windowEvent) = nullptr;
        char title[256];
        image icon;
        uint32_t width = 0, height = 0, xpos = 0, ypos = 0, ImGuiFlags = 0;
        bool hidden = false;
        bool iconified = false;
        int8_t vsyncMode = 0;
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
        friend void processEvents();

        static inline size_t instances;
        static inline std::vector<Window*> instancesList;
        GLFWwindow* window;
        windowData data;
        std::vector<windowEvent> eventQueue;
        ImGuiContext* ImContext;
        ImGuiIO* IO;

    public:
        /// @brief Window object empty default constuctor, can be used later with "createWindow()" method.
        Window();

        /// @brief Window object constructor.
        /// @param title The desired title.
        /// @param width The desired width.
        /// @param height The desired height.
        /// @param xpos The desired x position.
        /// @param ypos The desired y position.
        /// @param ImGuiFlags The desired imgui configuration flags.
        Window(const char* title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags);

        /// @brief Window object constuctor.
        /// @param data The desired "windowData" object for the window.
        Window(windowData data);

        ~Window();

        /// @brief Creates the window, you can use this in conjunction with the "Window()" constructor that tskes no arguments to create the window on screen later.
        /// @param title The desired title.
        /// @param width The desired width.
        /// @param height The desired height.
        /// @param xpos The desired x position.
        /// @param ypos The desired y position.
        /// @param ImGuiFlags The desired imgui configuration flags.
        void createWindow(const char* title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags = 0);

        /// @brief Creates the window, you can use this in conjunction with the "Window()" constructor that tskes no arguments to create the window on screen later.
        /// @param data The desired "windowData" object for the window.
        void createWindow(windowData data);

        void destroyWindow();

        /// @brief Starts the opengl and imgui frame for the window.
        void startFrame();

        /// @brief Hides the window, returns the sdl error code (0 is success).
        void endFrame();

        /// @brief Sets the event callback of the window that will be call every time there is an event.
        /// @param newEventCallback The desired function pointer for the event callback with signature "PNT::Window*, PNT::windowEvent" (use nullptr to clear callback).
        void setEventCallback(void(*newEventCallback)(Window*, windowEvent));

        /// @brief Sets the data struct of the window.
        /// @param newData The desired PNT::windowData for the window.
        void setWindowData(windowData newData);

        /// @brief Sets the window title.
        /// @param title The desired window title.
        void setTitle(const char* title);

        /// @brief Sets the window icon.
        /// @param image The desired PNT::image for the window icon.
        void setIcon(const image& image);

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
        void setVsyncMode(int8_t vsyncMode);

        /// @brief Sets the opengl clear color for the window.
        /// @param red The desired red channel.
        /// @param green The desired green channel.
        /// @param blue The desired blue channel.
        /// @param alpha The desired alpha channel.
        void setClearColor(float red, float green, float blue, float alpha);

        /// @brief Sets the should close status of the window (can be obtained with the "shouldClose();" method).
        /// @param shouldClose The desired should close status of the window.
        void setShouldClose(bool shouldClose);

        /// @brief Sets the required aspect ratio of the window.
        /// @param numerator The desired aspect ratio numerator (-1 for anything).
        /// @param denominator The desired aspect ratio denominator (-1 for anything).
        void setAspectRatio(uint32_t numerator, uint32_t denominator);

        /// @brief Pushes an event to the event stack.
        /// @param event The desired event to push, you can create events with the numerous "create...Event(...);" functions.
        /// @warning glfw has no event queue manipulation that I know of, so all custom events push by this function will be proccesed before glfw events.
        void pushEvent(windowEvent event);

        /// @brief Gets the window title.
        /// @return The window title (DO NOT MODIFY).
        const char* getTitle();

        /// @brief Gets the width of the window.
        /// @return The window width.
        uint16_t getWidth();

        /// @brief Gets the height of the window.
        /// @return The window height.
        uint16_t getHeight();

        /// @brief Gets the X position of the window.
        /// @return The window X position.
        uint16_t getXPos();

        /// @brief Gets the Y position of the window.
        /// @return The window Y position.
        uint16_t getYPos();

        /// @brief Gets the hidden state of the window.
        /// @return True if the window is hidden (not on screen of on taskbar), and false if otherwise.
        bool getHidden();

        /// @brief Gets the iconified state of the window.
        /// @return True if the window is minimised (on the taskbar), and false if otherwise.
        bool getIconified();

        /// @brief Check if the currect window should close.
        /// @return True if the window should close.
        bool shouldClose();
    };
}
