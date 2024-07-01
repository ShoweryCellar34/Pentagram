#include <windows.hpp>

namespace PNT {
    struct windowData {
        void(*eventCallback)(Window*, windowEvent) = nullptr;
        std::string title = "";
        image icon;
        uint16_t width = 0, height = 0;
        uint16_t xpos = 0, ypos = 0;
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

    Window::Window(const char* title = "Title", unsigned short width = 600, unsigned short height = 600, unsigned int xpos = 100, unsigned int ypos = 100, unsigned int ImGuiFlags = ImGuiConfigFlags_None) {
        instances++;

        data->title = title;
        data->width = width;
        data->height = height;

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

    Window::~Window() {
        instances--;

        ImGui::SetCurrentContext(ImGuiContext);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(ImGuiContext);

        glfwDestroyWindow(window);
    }

    void Window::startFrame() {
        glfwMakeContextCurrent(window);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(data->clearColor[0], data->clearColor[1], data->clearColor[2], data->clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::SetCurrentContext(ImGuiContext);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::endFrame() {
        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        GLFWwindow* backupContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupContext);
    }

    void Window::setEventCallback(void(*newEventCallback)(Window*, windowEvent)) {
        data->eventCallback = newEventCallback;
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    windowData* Window::getWindowData() {
        return data;
    }

    void Window::setWindowData(windowData* newdata) {
        data->eventCallback = newdata->eventCallback;
        setTitle(newdata->title.c_str());
        setDimentions(newdata->width, newdata->height);
        setPosition(newdata->xpos, newdata->ypos);
        setVsyncMode(newdata->vsyncMode);
        newdata->hidden ? show() : hide();
        newdata->iconified ? minimize() : maximize();
        setClearColor(newdata->clearColor[0], newdata->clearColor[1], newdata->clearColor[2], newdata->clearColor[3]);
    }

    void Window::setTitle(const char* title) {
        data->title = title;
        glfwSetWindowTitle(window, title);
    }

    bool Window::setIcon(const image& image) {
        if(image.valid()) {
            data->icon = image;
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

    void Window::setDimentions(uint16_t width, uint16_t height) {
        glfwSetWindowSize(window, width, height);
    }

    void Window::setPosition(uint16_t xpos, uint16_t ypos) {
        glfwSetWindowPos(window, xpos, ypos);
        callbackManagers::windowposCallbackManager(window, xpos, ypos);
    }

    void Window::hide() {
        glfwHideWindow(window);
    }
    void Window::show() {
        glfwShowWindow(window);
    }

    void Window::minimize() {
        glfwIconifyWindow(window);
    }
    void Window::maximize() {
        glfwRestoreWindow(window);
    }

    void Window::setVsyncMode(char vsyncMode) {
        data->vsyncMode = vsyncMode;
        glfwSwapInterval(vsyncMode);
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        data->clearColor[0] = red;
        data->clearColor[1] = green;
        data->clearColor[2] = blue;
        data->clearColor[3] = alpha;
    }

    void callbackManagers::keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createKeyEvent(key, scancode, action, mods));
        }
    }
    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createCharEvent(codepoint));
        }
    }
    void callbackManagers::dropCallbackManager(GLFWwindow* glfwWindow, int path_count, const char** paths) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createDropEvent(path_count, paths));
        }
    }
    void callbackManagers::scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createScrollEvent(xoffset, yoffset));
        }
    }
    void callbackManagers::cursorPosCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createCursorposEvent(xpos, ypos));
        }
    }
    void callbackManagers::windowposCallbackManager(GLFWwindow* glfwWindow, int xpos, int ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data->xpos = xpos;
        window->data->ypos = ypos;
        if(window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createWindowposEvent(xpos, ypos));
        }
    }
    void callbackManagers::windowsizeCallbackManager(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data->width = width;
        window->data->height = height;
        if(window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createWindowsizeEvent(width, height));
        }
    }
    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createMousebuttonEvent(button, action, mods));
        }
    }
    void callbackManagers::iconifyCallbackManager(GLFWwindow* glfwWindow, int iconified) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data->iconified = iconified;
        if(window->data->eventCallback != nullptr) {
            window->data->eventCallback(window, createIconifyEvent(iconified));
        }
    }
}
