#include <window.hpp>

#include <algorithm>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <error.hpp>
#include <event.hpp>
#include <image.hpp>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Window definitions.

    Window::Window() : window(nullptr), closed(true), frame(false), data(), eventQueue(), ImContext(nullptr), IO(nullptr) {
    }

    Window::Window(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) : window(nullptr), closed(true), frame(false), data(), eventQueue(), ImContext(nullptr), IO(nullptr) {
        createWindow(title, width, height, xpos, ypos, ImGuiFlags);
    }

    Window::Window(const windowData& data) : window(nullptr), closed(true), frame(false), data(), eventQueue(), ImContext(nullptr), IO(nullptr) {
        createWindow(data);
    }

    Window::~Window() {
        destroyWindow();
    }

    void Window::createWindowIntern(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        PNT_REQUIRE_INIT();

        logger.get()->info("[PNT]Creating window \"{}\"", title);

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
        closed = false;
    }

    void Window::createWindow(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        PNT_NO_WINDOW_ASSERT(window);

        createWindowIntern(title, width, height, xpos, ypos, ImGuiFlags);
    }

    void Window::createWindow(const windowData& data) {
        PNT_NO_WINDOW_ASSERT(window);

        createWindowIntern(data.title, data.width, data.height, data.xpos, data.ypos, data.ImGuiFlags);
    }

    void Window::destroyWindow() {
        if(!closed) {
            logger.get()->info("[PNT]Destroying window \"{}\"", data.title);

            instances--;
            instancesList.erase(std::find(instancesList.begin(), instancesList.end(), this));

            glfwDestroyWindow(window);

            window = nullptr;
            closed = true;
        }
    }

    void Window::startFrame() {
        newframe = std::chrono::steady_clock::now();

        PNT_WINDOW_ASSERT(window);
        PNT_ENDFRAME_ASSERT(frame);

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
        frame = true;
    }

    void Window::endFrame() {
        PNT_WINDOW_ASSERT(window);
        PNT_NEWFRAME_ASSERT(frame);

        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        GLFWwindow* backupContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupContext);
        frame = false;

        endframe = std::chrono::steady_clock::now();
        deltaTime = endframe - newframe;
    }

    void Window::setEventCallback(void(*newEventCallback)(Window*, windowEvent)) {
        PNT_WINDOW_ASSERT(window);

        data.eventCallback = newEventCallback;
    }

    void Window::pushEvent(windowEvent event) {
        PNT_WINDOW_ASSERT(window);

        logger.get()->debug("[PNT]Pushing event of type \"{}\" for window \"{}\"", event.getTypename(), data.title);

        eventQueue.push_back(event);
    }

    void Window::setUserPointer(void* pointer) {
        data.userPointer = pointer;
    }

    void Window::setWindowData(windowData newData) {
        PNT_WINDOW_ASSERT(window);

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
        PNT_WINDOW_ASSERT(window);

        strcpy(data.title, title);
        glfwSetWindowTitle(window, title);
    }

    void Window::setIcon(const image& icon) {
        PNT_WINDOW_ASSERT(window);

        logger.get()->debug("[PNT]Setting icon for window \"{}\"", data.title);

        if(icon.valid()) {
            GLFWimage glfwImage;
            glfwImage.width = icon.getWidth();
            glfwImage.height = icon.getHeight();
            glfwImage.pixels = icon.getPixels();
            glfwSetWindowIcon(window, 1, &glfwImage);
        } else {
            logger.get()->warn("[PNT]Icon failed to set");
            glfwSetWindowIcon(window, 0, nullptr);
        }
    }

    void Window::setDimentions(uint16_t width, uint16_t height) {
        PNT_WINDOW_ASSERT(window);

        glfwSetWindowSize(window, width, height);
    }

    void Window::setPosition(uint16_t xpos, uint16_t ypos) {
        PNT_WINDOW_ASSERT(window);

        glfwSetWindowPos(window, xpos, ypos);
        callbackManagers::windowposCallbackManager(window, xpos, ypos);
    }

    void Window::hide() {
        PNT_WINDOW_ASSERT(window);

        glfwHideWindow(window);
    }

    void Window::show() {
        PNT_WINDOW_ASSERT(window);

        glfwShowWindow(window);
    }

    void Window::minimize() {
        PNT_WINDOW_ASSERT(window);

        glfwIconifyWindow(window);
    }

    void Window::maximize() {
        PNT_WINDOW_ASSERT(window);

        glfwRestoreWindow(window);
    }

    void Window::setVsyncMode(int8_t vsyncMode) {
        PNT_WINDOW_ASSERT(window);

        data.vsyncMode = vsyncMode;
        glfwSwapInterval(vsyncMode);
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        PNT_WINDOW_ASSERT(window);

        data.clearColor[0] = red;
        data.clearColor[1] = green;
        data.clearColor[2] = blue;
        data.clearColor[3] = alpha;
    }

    void Window::setShouldClose(bool shouldClose) {
        PNT_WINDOW_ASSERT(window);

        glfwSetWindowShouldClose(window, shouldClose);
    }

    void Window::setAspectRatio(uint32_t numerator, uint32_t denominator) {
        PNT_WINDOW_ASSERT(window);

        logger.get()->debug("[PNT]Setting aspect ratio: {}, {} for window \"{}\"", numerator, denominator,data.title);

        glfwSetWindowAspectRatio(window, numerator, denominator);
    }

    std::chrono::duration<double> Window::getDeltaTime() const {
        return deltaTime;
    }

    void* Window::getUserPointer() const {
        return data.userPointer;
    }

    const char *Window::getTitle() const {
        PNT_WINDOW_ASSERT(window);

        return data.title;
    }

    uint16_t Window::getWidth() const {
        PNT_WINDOW_ASSERT(window);

        return data.width;
    }

    uint16_t Window::getHeight() const {
        PNT_WINDOW_ASSERT(window);

        return data.height;
    }

    uint16_t Window::getXPos() const {
        PNT_WINDOW_ASSERT(window);

        return data.xpos;
    }

    uint16_t Window::getYPos() const {
        PNT_WINDOW_ASSERT(window);

        return data.ypos;
    }

    bool Window::getHidden() const {
        PNT_WINDOW_ASSERT(window);

        return data.hidden;
    }

    bool Window::getIconified() const {
        PNT_WINDOW_ASSERT(window);

        return data.iconified;
    }

    bool Window::shouldClose() const {
        PNT_WINDOW_ASSERT(window);

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
