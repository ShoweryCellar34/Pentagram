#include <window.hpp>

#include <algorithm>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <event.hpp>

namespace PNT {
    // Window definitions.

    Window::Window(const char* title = "Title", uint32_t width = 600, uint32_t height = 600, uint32_t xpos = 100, uint32_t ypos = 100, uint32_t ImGuiFlags = ImGuiConfigFlags_None) {
        instancesList.push_back(this);
        instances++;

        strcpy(data.title, title);
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

        ImContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(ImContext);
        IO = &ImGui::GetIO();
        IO->ConfigFlags |= ImGuiFlags;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(nullptr);
        ImGui::StyleColorsDark();
    }

    Window::~Window() {
        if(!destroyed) {
            instances--;
            instancesList.erase(std::find(instancesList.begin(), instancesList.end(), this));

            glfwDestroyWindow(window);

            destroyed = true;
        }
    }

    void Window::startFrame() {
        glfwMakeContextCurrent(window);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(data.clearColor[0], data.clearColor[1], data.clearColor[2], data.clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::SetCurrentContext(ImContext);
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
        data.eventCallback = newEventCallback;
    }

    void Window::setWindowData(windowData newData) {
        data.eventCallback = newData.eventCallback;
        setTitle(newData.title);
        setDimentions(newData.width, newData.height);
        setPosition(newData.xpos, newData.ypos);
        setVsyncMode(newData.vsyncMode);
        newData.hidden ? show() : hide();
        newData.iconified ? minimize() : maximize();
        setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
    }

    void Window::setTitle(const char* title) {
        strcpy(data.title, title);
        glfwSetWindowTitle(window, title);
    }

    bool Window::setIcon(const image& image) {
        if(image.valid()) {
            data.icon = image;
            data.icon.unloadOffGPU();
            GLFWimage glfwImage;
            glfwImage.width = image.getWidth();
            glfwImage.height = image.getHeight();
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

    void Window::setVsyncMode(uint8_t vsyncMode) {
        data.vsyncMode = vsyncMode;
        glfwSwapInterval(vsyncMode);
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        data.clearColor[0] = red;
        data.clearColor[1] = green;
        data.clearColor[2] = blue;
        data.clearColor[3] = alpha;
    }

    void Window::setShouldClose(bool shouldClose) {
        glfwSetWindowShouldClose(window, shouldClose);
    }

    void Window::pushEvent(windowEvent event) {
        eventQueue.push_back(event);
    }

    const char *Window::getTitle()
    {
        return data.title;
    }

    uint16_t Window::getWidth() {
        return data.width;
    }

    uint16_t Window::getHeight() {
        return data.height;
    }

    uint16_t Window::getXPos() {
        return data.xpos;
    }

    uint16_t Window::getYPos() {
        return data.ypos;
    }

    bool Window::getHidden() {
        return data.hidden;
    }

    bool Window::getIconified() {
        return data.iconified;
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    // Callback definitions.

    void callbackManagers::keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createKeyEvent(key, scancode, action, mods));
        }
    }

    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureKeyboard && window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createCharEvent(codepoint));
        }
    }

    void callbackManagers::dropCallbackManager(GLFWwindow* glfwWindow, int path_count, const char** paths) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createDropEvent(path_count, paths));
        }
    }

    void callbackManagers::scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createScrollEvent(xoffset, yoffset));
        }
    }

    void callbackManagers::cursorPosCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createCursorposEvent(xpos, ypos));
        }
    }

    void callbackManagers::windowposCallbackManager(GLFWwindow* glfwWindow, int xpos, int ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.xpos = xpos;
        window->data.ypos = ypos;
        if(window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createWindowposEvent(xpos, ypos));
        }
    }

    void callbackManagers::windowsizeCallbackManager(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.width = width;
        window->data.height = height;
        if(window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createWindowsizeEvent(width, height));
        }
    }

    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->IO->WantCaptureMouse && window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createMousebuttonEvent(button, action, mods));
        }
    }

    void callbackManagers::iconifyCallbackManager(GLFWwindow* glfwWindow, int iconified) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->data.iconified = iconified;
        if(window->data.eventCallback != nullptr) {
            window->data.eventCallback(window, createIconifyEvent(iconified));
        }
    }
}
