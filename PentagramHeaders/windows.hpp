#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <enumerations.hpp>
#include <events.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT {
    class Window;

    struct windowData {
        void(*startFrameCallback)(Window*) = nullptr;
        void(*endFrameCallback)(Window*) = nullptr;
        void(*userEventCallback)(Window*, windowEvent) = nullptr;
        std::string title = "";
        unsigned short width = 0, height = 0;
        unsigned short x = 0, y = 0;
        unsigned char visiblity = 0;
        unsigned char vsyncMode = 0;
        float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    // Processes all pending events.
    void processEvents() {
        glfwPollEvents();
    }

    class callbackManagers {
    private:
        friend class Window;

        static void keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
        static void charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint);
        static void dropCallbackManager(GLFWwindow* window, int path_count, const char* paths[]);
        static void scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset);
        static void cursorposCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos);
        static void mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods);
    };

    class Window {
    private:
        // Functions
        friend class callbackManagers;
        friend void deinit();
    public:
        // Constructor/Destructor
        Window(const char* title = "Title", unsigned short width = 600, unsigned short height = 600) {
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

            glfwSetKeyCallback(window, callbackManagers::keyCallbackManager);
            glfwSetCharCallback(window, callbackManagers::charCallbackManager);
            glfwSetDropCallback(window, callbackManagers::dropCallbackManager);
            //glfwSetErrorCallback(callbackManagers::);
            glfwSetScrollCallback(window, callbackManagers::scrollCallbackManager);
            //glfwSetMonitorCallback(callbackManagers::);
            //glfwSetCharModsCallback(window, callbackManagers::);
            //glfwSetJoystickCallback(callbackManagers::);
            glfwSetCursorPosCallback(window, callbackManagers::cursorposCallbackManager);
            //glfwSetWindowPosCallback(window, callbackManagers::);
            //glfwSetWindowSizeCallback(window, callbackManagers::);
            //glfwSetCursorEnterCallback(window, callbackManagers::);
            glfwSetMouseButtonCallback(window, callbackManagers::mousebuttonCallbackManager);
            //glfwSetWindowCloseCallback(window, callbackManagers::);
            //glfwSetWindowFocusCallback(window, callbackManagers::);
            //glfwSetWindowIconifyCallback(window, callbackManagers::);
            //glfwSetWindowRefreshCallback(window, callbackManagers::);
            //glfwSetWindowMaximizeCallback(window, callbackManagers::);
            //glfwSetFramebufferSizeCallback(window, callbackManagers::);
            //glfwSetWindowContentScaleCallback(window, callbackManagers::);

            ImGuiContext = ImGui::CreateContext();
            IO = &ImGui::GetIO();
            IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            IO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
            if(data.startFrameCallback != nullptr) {data.startFrameCallback(this);}
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
            if(data.endFrameCallback != nullptr) {data.endFrameCallback(this);}
        }

        // Sets the callback for the specified window event (use nullptr to clear callback).
        void setCallback(unsigned char callbackID, void (*newcallback)(Window*)) {
            switch(callbackID) {
        	case PNT_CALLBACK_FLAGS_STARTFRAME:
                data.startFrameCallback = newcallback;
                break;

            case PNT_CALLBACK_FLAGS_ENDFRAME:
                data.endFrameCallback = newcallback;
                break;

            default:
                break;
            }
        }

        // Sets the event callback of the window (use nullptr to clear callback).
        void setEventCallback(void(*newcallback)(Window*, windowEvent)) {data.userEventCallback = newcallback;}

        // Returns the data struct of the window.
        windowData getWindowData() {return data;}

        // Sets the data struct of the window.
        void setWindowData(windowData newData) {
            data.startFrameCallback = newData.startFrameCallback;
            data.endFrameCallback = newData.endFrameCallback;
            data.userEventCallback = newData.userEventCallback;
            setTitle(newData.title.c_str());
            setDimentions(newData.width, newData.height);
            setPosition(newData.x, newData.y);
            setVsyncMode(newData.vsyncMode);
            setVisiblity(newData.visiblity);
            setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
        }

        // Sets the title of the window.
        void setTitle(const char* title) {
            data.title = title;
            glfwSetWindowTitle(window, title);
        }

        // Sets the width and height of the window.
        void setDimentions(unsigned short width, unsigned short height) {
            glfwSetWindowSize(window, width, height);
        }

        // Sets the x and y coordinates of the window.
        void setPosition(unsigned int x, unsigned int y) {
            glfwSetWindowPos(window, x, y);
        }

        // Sets the visiblity of the window.
        void setVisiblity(bool visiblity) {
            visiblity == true ? glfwShowWindow(window) : glfwHideWindow(window);
        }

        // Sets the vsync mode of the window (0 = off, 1 = on, -1 = adaptive).
        void setVsyncMode(char vsyncMode) {
            data.vsyncMode = vsyncMode;
            glfwSwapInterval(vsyncMode);
        }

        // Sets the opengl clear color for the window.
        void setClearColor(float red = -1, float green = -1, float blue = -1, float alpha = -1) {
            if(red != -1) data.clearColor[0] = red;
            if(green != -1) data.clearColor[1] = green;
            if(blue != -1) data.clearColor[2] = blue;
            if(alpha != -1) data.clearColor[3] = alpha;
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
        if(!window->IO->WantCaptureKeyboard) {window->data.userEventCallback(window, createKeyEvent(key, scancode, action, mods));}
    }
    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse) {window->data.userEventCallback(window, createCharEvent(codepoint));}
    }
    void callbackManagers::dropCallbackManager(GLFWwindow* glfwWindow, int path_count, const char* paths[]) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse) {window->data.userEventCallback(window, createDropEvent(path_count, paths));}
    }
    void callbackManagers::scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse) {window->data.userEventCallback(window, createScrollEvent(xoffset, yoffset));}
    }
    void callbackManagers::cursorposCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse) {window->data.userEventCallback(window, createCursorposEvent(xpos, ypos));}
    }
    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse) {window->data.userEventCallback(window, createMousebuttonEvent(button, action, mods));}
    }
}
