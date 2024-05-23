#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <enumerations.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT {
    class Window;
    struct windowData {
        std::vector<std::function<void(PNT::Window*)>> userCallbacks = {nullptr};
        std::vector<std::function<void(PNT::Window*)>> userEventCallbacks = {nullptr};
        std::string title = "";
        unsigned short width = 0, height = 0;
        unsigned short x = 0, y = 0;
        unsigned char visiblity = 0;
        unsigned char vsyncMode = 0;
        float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        windowData() {
            userCallbacks.reserve(PNT_CALLBACK_FLAGS_COUNT);
            userEventCallbacks.reserve(PNT_EVENT_CALLBACK_FLAGS_COUNT);
        }
    };

    void keyCallbackManager(GLFWwindow*, int, int, int, int);
    void scrollCallbackManager(GLFWwindow*, double, double);
    void cursorposCallbackManager(GLFWwindow*, double, double);
    void mousebuttonCallbackManager(GLFWwindow*, int, int, int);
    class Window {
    public:
        // Constructor/Destructor
        Window(const char* title = "Title", unsigned short width = 600, unsigned short height = 600) {
            instances++;
            instanceList[window] = this;

            data.title = title;
            data.width = width;
            data.height = height;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwSetKeyCallback(window, keyCallbackManager);
            glfwSetScrollCallback(window, scrollCallbackManager);
            glfwSetCursorPosCallback(window, cursorposCallbackManager);
            glfwSetMouseButtonCallback(window, mousebuttonCallbackManager);

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, keyCallbackManager);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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
            auto instanceID = std::find(instanceList.begin(), instanceList.end(), window);
            instanceList.erase(instanceID);
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
            if(*(data.userCallbacks[0].target<void(*)(Window*)>()) != nullptr) {
                (*(data.userCallbacks[0].target<void(*)(Window*)>()))(this);
            }
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
            if(*(data.userCallbacks[1].target<void(*)(Window*)>()) != nullptr)
            {
                (*(data.userCallbacks[1].target<void(*)(Window*)>()))(this);
            }
        }

        // Sets the callback for the specified window event (use nullptr to clear callback).
        void setCallback(unsigned char callbackID, void (*newcallback)(Window*)) {
            switch(callbackID) {
        	case PNT_CALLBACK_FLAGS_STARTFRAME:
                data.userCallbacks[0] = newcallback;
                break;

            case PNT_CALLBACK_FLAGS_ENDFRAME:
                data.userCallbacks[1] = newcallback;
                break;

            default:
                break;
            }
        }

        // Sets the callback for the specified input event (use nullptr to clear callback).
        void setEventCallback(unsigned char callbackID, void (*newcallback)(Window*)) {
            switch(callbackID) {
            case PNT_EVENT_CALLBACK_FLAGS_KEYBOARD:
                data.userEventCallbacks[0] = newcallback;
                break;

            case PNT_EVENT_CALLBACK_FLAGS_SCROLL:
                data.userEventCallbacks[1] = newcallback;
                break;

            case PNT_EVENT_CALLBACK_FLAGS_CURSORPOS:
                data.userEventCallbacks[2] = newcallback;
                break;

            case PNT_EVENT_CALLBACK_FLAGS_MOUSEBUTTON:
                data.userEventCallbacks[3] = newcallback;
                break;

            default:
                break;
            }
        }

        // Returns the data struct of the window.
        windowData getWindowData() {return data;}

        // Sets the data struct of the window.
        void setWindowData(windowData newData) {
            setCallback(PNT_CALLBACK_FLAGS_STARTFRAME, *(data.userCallbacks[0].target<void(*)(Window*)>()));
            setCallback(PNT_CALLBACK_FLAGS_ENDFRAME, *(data.userCallbacks[1].target<void(*)(Window*)>()));
            setEventCallback(PNT_EVENT_CALLBACK_FLAGS_KEYBOARD, *(data.userEventCallbacks[0].target<void(*)(Window*)>()));
            setEventCallback(PNT_EVENT_CALLBACK_FLAGS_SCROLL, *(data.userEventCallbacks[1].target<void(*)(Window*)>()));
            setEventCallback(PNT_EVENT_CALLBACK_FLAGS_MOUSEBUTTON, *(data.userEventCallbacks[2].target<void(*)(Window*)>()));
            setTitle(newData.title.c_str());
            setDimentions(newData.width, newData.height);
            setPosition(newData.x, newData.y);
            setVsyncMode(newData.vsyncMode);
            setVisiblity(newData.visiblity);
            setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
        }

        // Sets the title of the window.
        void setTitle(const char* title) {
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

        // Sets the opengl clear color for the window (-1 = unchanged).
        void setClearColor(float red = -1, float green = -1, float blue = -1, float alpha = -1) {
            if(red != -1) data.clearColor[0] = red;
            if(green != -1) data.clearColor[1] = green;
            if(blue != -1) data.clearColor[2] = blue;
            if(alpha != -1) data.clearColor[3] = alpha;
        }
    private:
        // SDL data
        GLFWwindow* window;

        // Window data
        windowData data;

        // ImGui data
        ImGuiContext* ImGuiContext;
        ImGuiIO* IO;

        // other data
        static inline int instances;
        static inline std::unordered_map<GLFWwindow*, Window*> instanceList;
        char instanceID;

        // Functions
        friend void keyCallbackManager(GLFWwindow* window, int key, int scancode, int action, int mods);
        void keyEvent(int key, int scancode, int action, int mods) {
            if(!IO->WantCaptureKeyboard) {
                data.userCallbacks[0](this);
            }
        }

        friend void scrollCallbackManager(GLFWwindow*, double, double);
        void scrollEvent(double xoffset, double yoffset) {
            if(!IO->WantCaptureMouse) {
                data.userCallbacks[1](this);
            }
        }

        friend void cursorposCallbackManager(GLFWwindow*, double, double);
        void cursorposEvent(double xpos, double ypos) {
            if(!IO->WantCaptureMouse) {
                data.userCallbacks[2](this);
            }
        }

        friend void mousebuttonCallbackManager(GLFWwindow*, int, int, int);
        void mousebuttonEvent(int button, int action, int mods) {
            if(!IO->WantCaptureMouse) {
                data.userCallbacks[2](this);
            }
        }
    };

    void keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = Window::instanceList.at(glfwWindow);
        window->keyEvent(key, scancode, action, mods);
    }
    void scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = Window::instanceList.at(glfwWindow);
        window->scrollEvent(xoffset, yoffset);
    }
    void cursorposCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = Window::instanceList.at(glfwWindow);
        window->cursorposEvent(xpos, ypos);
    }
    void mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = Window::instanceList.at(glfwWindow);
        window->mousebuttonEvent(button, action, mods);
    }
}
