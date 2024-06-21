#pragma once

#include "includes.hpp"
#include "enumerations.hpp"
#include "event.hpp"
#include "image.hpp"

namespace PNT {
    class Window;

    struct windowData {
        void(*eventCallback)(Window*, windowEvent) = nullptr;
        std::string title = "";
        image icon;
        unsigned short width = 0, height = 0;
        unsigned short xpos = 0, ypos = 0;
        bool hidden = false;
        bool iconified = false;
        unsigned char vsyncMode = 0;
        float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    class callbackManagers {
    private:
        friend class Window;

        static void keyCallbackManager(GLFWwindow*, int, int, int, int);
        static void charCallbackManager(GLFWwindow*, unsigned int);
        static void dropCallbackManager(GLFWwindow*, int, const char* []);
        static void scrollCallbackManager(GLFWwindow*, double, double);
        static void cursorPosCallbackManager(GLFWwindow*, double, double);
        static void windowposCallbackManager(GLFWwindow*, int, int);
        static void windowsizeCallbackManager(GLFWwindow*, int, int);
        static void mousebuttonCallbackManager(GLFWwindow*, int, int, int);
        static void iconifyCallbackManager(GLFWwindow*, int);
    };

    class Window {
    private:
        // Functions
        friend class callbackManagers;
        friend void deinit();
    public:
        // Constructor/Destructor
        Window(const char* title = "Title", unsigned short width = 600, unsigned short height = 600, unsigned int xpos = 100, unsigned int ypos = 100, unsigned int ImGuiFlags = ImGuiConfigFlags_None) {
            instances++;

            data.title = title;
            data.width = width;
            data.height = height;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwSetWindowUserPointer(window, this);
            glfwMakeContextCurrent(window);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            setPosition(xpos, ypos);

            glfwSetKeyCallback(window, callbackManagers::keyCallbackManager);
            glfwSetCharCallback(window, callbackManagers::charCallbackManager);
            glfwSetDropCallback(window, callbackManagers::dropCallbackManager);
            //glfwSetErrorCallback(callbackManagers::);
            glfwSetScrollCallback(window, callbackManagers::scrollCallbackManager);
            //glfwSetMonitorCallback(callbackManagers::);
            //glfwSetCharModsCallback(window, callbackManagers::);
            //glfwSetJoystickCallback(callbackManagers::);
            glfwSetCursorPosCallback(window, callbackManagers::cursorPosCallbackManager);
            glfwSetWindowPosCallback(window, callbackManagers::windowposCallbackManager);
            glfwSetWindowSizeCallback(window, callbackManagers::windowsizeCallbackManager);
            //glfwSetCursorEnterCallback(window, callbackManagers::);
            glfwSetMouseButtonCallback(window, callbackManagers::mousebuttonCallbackManager);
            //glfwSetWindowCloseCallback(window, callbackManagers::);
            //glfwSetWindowFocusCallback(window, callbackManagers::);
            glfwSetWindowIconifyCallback(window, callbackManagers::iconifyCallbackManager);
            //glfwSetWindowRefreshCallback(window, callbackManagers::);
            //glfwSetWindowMaximizeCallback(window, callbackManagers::);
            //glfwSetFramebufferSizeCallback(window, callbackManagers::);
            //glfwSetWindowContentScaleCallback(window, callbackManagers::);

            ImGuiContext = ImGui::CreateContext();
            IO = &ImGui::GetIO();
            IO->ConfigFlags |= ImGuiFlags;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 460");
            ImGui::StyleColorsDark();
        }
        ~Window() {
            instances--;

            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(ImGuiContext);

            glfwDestroyWindow(window);
        }

        // Starts the opengl and imgui frame for the window, returns the sdl error code (0 is success)..
        void startFrame() {
            glfwMakeContextCurrent(window);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(data.clearColor[0], data.clearColor[1], data.clearColor[2], data.clearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        // Hides the window, returns the sdl error code (0 is success). 
        void endFrame() {
            ImGui::Render();
            ImGuiIO& io = ImGui::GetIO();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }

        /// @brief Sets the event callback of the window.
        /// @param newEventCallback The function pointer to the event callback with signature 'PNT::Window*, PNT::windowEvent' (use nullptr to clear callback).
        void setEventCallback(void(*newEventCallback)(Window*, windowEvent)) {
            data.eventCallback = newEventCallback;
        }

        /// @brief Check if the currect window should close.
        /// @return True if the window should close.
        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }

        /// @brief Get the window data.
        /// @return A PNT::windowData struct.
        windowData getWindowData() {
            return data;
        }

        // 

        /// @brief Sets the data struct of the window.
        /// @param newData A PNT::windowData struct to override the current one.
        void setWindowData(windowData newData) {
            data.eventCallback = newData.eventCallback;
            setTitle(newData.title.c_str());
            setDimentions(newData.width, newData.height);
            setPosition(newData.xpos, newData.ypos);
            setVsyncMode(newData.vsyncMode);
            newData.hidden ? show() : hide();
            newData.iconified ? minimize() : maximize();
            setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
        }

        /// @brief Sets the window title.
        /// @param title Window title.
        void setTitle(const char* title) {
            data.title = title;
            glfwSetWindowTitle(window, title);
        }

        /// @brief Sets the window icon.
        /// @param image The PNT::image that will be used for the icon.
        /// @return Will return false if the image is invalid, this happens if the contents of the image are null.
        bool setIcon(const image& image) {
            if(image.valid()) {
                data.icon = image;
                GLFWimage glfwImage;
                glfwImage.width = image.getDimentions().first;
                glfwImage.height = image.getDimentions().second;
                glfwImage.pixels = image.getPixels();
                glfwSetWindowIcon(window, 1, &glfwImage);
                return 1;
            } else {
                glfwSetWindowIcon(window, 0, nullptr);
                return 0;
            }
        }

        // Sets the width and height of the window.
        void setDimentions(unsigned short width, unsigned short height) {
            glfwSetWindowSize(window, width, height);
        }

        // Sets the x and y coordinates of the window.
        void setPosition(unsigned int xpos, unsigned int ypos) {
            glfwSetWindowPos(window, xpos, ypos);
            callbackManagers::windowposCallbackManager(window, xpos, ypos);
        }

        // Sets the visiblity of the window.
        void hide() {
            glfwHideWindow(window);
        }
        void show() {
            glfwShowWindow(window);
        }

        // Minimizes and maximizes the window.
        void minimize() {glfwIconifyWindow(window);}
        void maximize() {glfwRestoreWindow(window);}

        // Sets the vsync mode of the window (0 = off, 1 = on, -1 = adaptive).
        void setVsyncMode(char vsyncMode) {
            data.vsyncMode = vsyncMode;
            glfwSwapInterval(vsyncMode);
        }

        // Sets the opengl clear color for the window.
        void setClearColor(float red, float green, float blue, float alpha) {
            data.clearColor[0] = red;
            data.clearColor[1] = green;
            data.clearColor[2] = blue;
            data.clearColor[3] = alpha;
        }
    private:
        // GLFW window.
        GLFWwindow* window;

        // Window data
        windowData data;

        // ImGui data
        ImGuiContext* ImGuiContext;
        ImGuiIO* IO;

        // other data
        static inline int instances;
    };

    void callbackManagers::keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data.eventCallback != nullptr) {window->data.eventCallback(window, createKeyEvent(key, scancode, action, mods));}
    }
    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data.eventCallback != nullptr) {window->data.eventCallback(window, createCharEvent(codepoint));}
    }
    void callbackManagers::dropCallbackManager(GLFWwindow* glfwWindow, int path_count, const char* paths[]) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(window->data.eventCallback != nullptr) window->data.eventCallback(window, createDropEvent(path_count, paths));
    }
    void callbackManagers::scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {window->data.eventCallback(window, createScrollEvent(xoffset, yoffset));}
    }
    void callbackManagers::cursorPosCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {window->data.eventCallback(window, createCursorposEvent(xpos, ypos));}
    }
    void callbackManagers::windowposCallbackManager(GLFWwindow* glfwWindow, int xpos, int ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.xpos = xpos;
        window->data.ypos = ypos;
        if(window->data.eventCallback != nullptr) window->data.eventCallback(window, createWindowposEvent(xpos, ypos));
    }
    void callbackManagers::windowsizeCallbackManager(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.width = width;
        window->data.height = height;
        if(window->data.eventCallback != nullptr) window->data.eventCallback(window, createWindowsizeEvent(width, height));
    }
    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {window->data.eventCallback(window, createMousebuttonEvent(button, action, mods));}
    }
    void callbackManagers::iconifyCallbackManager(GLFWwindow* glfwWindow, int iconified) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.iconified = iconified;
        if(window->data.eventCallback != nullptr) window->data.eventCallback(window, createIconifyEvent(iconified));
    }
}
