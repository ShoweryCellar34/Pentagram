#pragma once

#include <includes.hpp>
#include <logger.hpp>
#include <enumerations.hpp>
#include <utilities/ptrToChar.hpp>

namespace PNT {
    struct windowData {
        std::string title = "";
        short width = -1, height = -1;
        short x = -1, y = -1;
        char vsyncMode = -1;
        char visiblity = -1;
        float clearColor[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
    };

    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    class Window {
    public:
        // Constructor/Destructor
        Window(const char* title = "Title", unsigned short width = 600, unsigned short height = 600) {
            instances++;
            instanceList[window] = this;

            data.title = (char* )title;
            data.width = width;
            data.height = height;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, keyboardCallback);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            ImGuiContext = ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
        void startFrame()
        {
            glfwMakeContextCurrent(window);
            int width, height;
            glfwGetFramebufferSize(window,& width,& height);
            glViewport(0, 0, width, height);
            glClearColor(data.clearColor[0], data.clearColor[1], data.clearColor[2], data.clearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::SetCurrentContext(ImGuiContext);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            if(startFrameListener != nullptr)
            {
                startFrameListener(this);
            }
        }

        // Hides the window, returns the sdl error code (0 is success). 
        void endFrame()
        {
            ImGui::Render();
            ImGuiIO& io = ImGui::GetIO();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
            if(endFrameListener != nullptr)
            {
                endFrameListener(this);
            }
        }

        // Sets the listener for the specified event (use nullptr to clear listener).
        void setListener(unsigned char listenerID, void (*newListener)(Window* )) {
            switch(listenerID) {
        	case PNT_LISTENER_FLAGS_STARTFRAME:
                startFrameListener = newListener;
                break;

            case PNT_LISTENER_FLAGS_ENDFRAME:
                endFrameListener = newListener;
                break;

            case PNT_LISTENER_FLAGS_KEYBOARDEVENT:
                keyboardEventListener = newListener;
                break;

            case PNT_LISTENER_FLAGS_MOUSEEVENT:
                mouseEventListener = newListener;
                break;
            default:
                break;
            }
        }

        // Returns the data struct of the window.
        windowData getWindowData() {return data;}

        // Sets the data struct of the window.
        void setWindowData(windowData newData) {
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

        // listener data
        void (*startFrameListener)(Window* ) = nullptr;
        void (*endFrameListener)(Window* ) = nullptr;
        void (*keyboardEventListener)(Window* ) = nullptr;
        void (*mouseEventListener)(Window* ) = nullptr;

        // other data
        static inline int instances;
        static inline std::map<GLFWwindow*, Window*> instanceList;
        char instanceID;

        // Functions
        friend void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void keyboardEvent(int key, int scancode, int action, int mods) {}
    };

    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window::instanceList.at(window)->keyboardEvent(key, scancode, action, mods);
    }
}
