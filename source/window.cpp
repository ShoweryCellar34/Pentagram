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

    Window::Window() = default;

    Window::Window(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        createWindow(title, width, height, xpos, ypos, ImGuiFlags);
    }

    Window::Window(windowData data) {
        createWindow(data);
    }

    Window::~Window() {
        destroyWindow();
    }

    void Window::createWindow(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        if(!window) {
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
    }

    void Window::createWindow(windowData data) {
        if(!window) {
            instancesList.push_back(this);
            instances++;

            strcpy(this->data.title, data.title);
            this->data.width = data.width;
            this->data.height = data.height;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(data.width, data.height, data.title, NULL, NULL);
            glfwSetWindowUserPointer(window, this);
            glfwMakeContextCurrent(window);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            setPosition(data.xpos, data.ypos);

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
            IO->ConfigFlags |= data.ImGuiFlags;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(nullptr);
            ImGui::StyleColorsDark();
        }
    }

    void Window::destroyWindow()
    {
        if(window) {
            instances--;
            instancesList.erase(std::find(instancesList.begin(), instancesList.end(), this));

            glfwDestroyWindow(window);

            window = nullptr;
        }
    }

    void Window::startFrame()
    {
        if(window) {
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
    }

    void Window::endFrame() {
        if(window) {
            ImGui::Render();
            ImGuiIO& io = ImGui::GetIO();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }

    void Window::setEventCallback(void(*newEventCallback)(Window*, windowEvent)) {
        if(window) {
            data.eventCallback = newEventCallback;
        }
    }

    void Window::setWindowData(windowData newData) {
        if(window) {
            data.eventCallback = newData.eventCallback;
            setTitle(newData.title);
            setDimentions(newData.width, newData.height);
            setPosition(newData.xpos, newData.ypos);
            setVsyncMode(newData.vsyncMode);
            newData.hidden ? show() : hide();
            newData.iconified ? minimize() : maximize();
            setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
        }
    }

    void Window::setTitle(const char* title) {
        if(window) {
            strcpy(data.title, title);
            glfwSetWindowTitle(window, title);
        }
    }

    void Window::setIcon(const image& image) {
        if(window) {
            if(image.valid()) {
                data.icon = image;
                data.icon.unloadOffGPU();
                GLFWimage glfwImage;
                glfwImage.width = image.getWidth();
                glfwImage.height = image.getHeight();
                glfwImage.pixels = image.getPixels();
                glfwSetWindowIcon(window, 1, &glfwImage);
            } else {
                glfwSetWindowIcon(window, 0, nullptr);
            }
        }
    }

    void Window::setDimentions(uint16_t width, uint16_t height) {
        if(window) {
            glfwSetWindowSize(window, width, height);
        }
    }

    void Window::setPosition(uint16_t xpos, uint16_t ypos) {
        if(window) {
            glfwSetWindowPos(window, xpos, ypos);
            callbackManagers::windowposCallbackManager(window, xpos, ypos);
        }
    }

    void Window::hide() {
        if(window) {
            glfwHideWindow(window);
        }
    }

    void Window::show() {
        if(window) {
            glfwShowWindow(window);
        }
    }

    void Window::minimize() {
        if(window) {
            glfwIconifyWindow(window);
        }
    }

    void Window::maximize() {
        if(window) {
            glfwRestoreWindow(window);
        }
    }

    void Window::setVsyncMode(int8_t vsyncMode) {
        if(window) {
            data.vsyncMode = vsyncMode;
            glfwSwapInterval(vsyncMode);
        }
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        if(window) {
            data.clearColor[0] = red;
            data.clearColor[1] = green;
            data.clearColor[2] = blue;
            data.clearColor[3] = alpha;
        }
    }

    void Window::setShouldClose(bool shouldClose) {
        if(window) {
            glfwSetWindowShouldClose(window, shouldClose);
        }
    }

    void Window::pushEvent(windowEvent event) {
        if(window) {
            eventQueue.push_back(event);
        }
    }

    void Window::setAspectRatio(uint32_t numerator, uint32_t denominator) {
        if(window) {
            glfwSetWindowAspectRatio(window, numerator, denominator);
        }
    }

    const char *Window::getTitle() {
        if(window) {
            return data.title;
        }
        return 0;
    }

    uint16_t Window::getWidth() {
        if(window) {
            return data.width;
        }
        return 0;
    }

    uint16_t Window::getHeight() {
        if(window) {
            return data.height;
        }
        return 0;
    }

    uint16_t Window::getXPos() {
        if(window) {
            return data.xpos;
        }
        return 0;
    }

    uint16_t Window::getYPos() {
        if(window) {
            return data.ypos;
        }
        return 0;
    }

    bool Window::getHidden() {
        if(window) {
            return data.hidden;
        }
        return 0;
    }

    bool Window::getIconified() {
        if(window) {
            return data.iconified;
        }
        return 0;
    }

    bool Window::shouldClose() {
        if(window) {
            return glfwWindowShouldClose(window);
        }
        return 0;
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
