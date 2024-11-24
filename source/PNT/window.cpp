#include <PNT/window.hpp>

#include <algorithm>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <PNT/error.hpp>
#include <PNT/event.hpp>

namespace PNT {
    extern bool initialized;
    extern std::shared_ptr<spdlog::logger> logger;

    // Window definitions.

    Window::Window() : m_window(nullptr), m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), m_IO(nullptr) {
    }

    Window::Window(const std::string& title, int width, int height, int xpos, int ypos, int ImGuiFlags) : m_window(nullptr), m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), m_IO(nullptr) {
        createWindow(title, width, height, xpos, ypos, ImGuiFlags);
    }

    Window::Window(const windowData& data) : m_window(nullptr), m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), m_IO(nullptr) {
        createWindow(data);
    }

    Window::~Window() {
        destroyWindow();
    }

    void Window::createWindowIntern(const std::string& title, int width, int height, int xpos, int ypos, int ImGuiFlags) {
        if(!initialized) {
            throw exception("Pentagram not initalized.", errorCodes::PNT_ERROR);
        }

        m_openglContext = new GladGLContext;

        logger.get()->info("[PNT]Creating window \"{}\"", title);

        m_instancesList.emplace_back(this);
        m_instances++;

        this->m_data.title = title;
        m_data.width = width;
        m_data.height = height;
        m_data.ImGuiFlags = ImGuiFlags;

        // Intel integrted graphics no likie-like these lines
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwSetWindowUserPointer(m_window, this);
        glfwMakeContextCurrent(m_window);
        gladLoadGLContext(m_openglContext, (GLADloadfunc)glfwGetProcAddress);

        glfwSetKeyCallback(m_window, callbackManagers::keyCallbackManager);
        glfwSetCharCallback(m_window, callbackManagers::charCallbackManager);
        glfwSetDropCallback(m_window, callbackManagers::dropCallbackManager);
        glfwSetScrollCallback(m_window, callbackManagers::scrollCallbackManager);
        //glfwSetCharModsCallback(m_window, callbackManagers::);
        //glfwSetJoystickCallback(callbackManagers::);
        glfwSetCursorPosCallback(m_window, callbackManagers::cursorPosCallbackManager);
        glfwSetWindowPosCallback(m_window, callbackManagers::windowposCallbackManager);
        glfwSetWindowSizeCallback(m_window, callbackManagers::windowsizeCallbackManager);
        glfwSetCursorEnterCallback(m_window, callbackManagers::cursorEnterCallback);
        glfwSetMouseButtonCallback(m_window, callbackManagers::mousebuttonCallbackManager);
        //glfwSetWindowCloseCallback(m_window, callbackManagers::);
        glfwSetWindowFocusCallback(m_window, callbackManagers::windowFocusCallback);
        glfwSetWindowIconifyCallback(m_window, callbackManagers::iconifyCallbackManager);
        //glfwSetWindowRefreshCallback(m_window, callbackManagers::);
        //glfwSetWindowMaximizeCallback(m_window, callbackManagers::);
        //glfwSetFramebufferSizeCallback(m_window, callbackManagers::);
        //glfwSetWindowContentScaleCallback(m_window, callbackManagers::);

        m_ImContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(m_ImContext);
        m_IO = &ImGui::GetIO();
        m_IO->ConfigFlags |= ImGuiFlags;
        ImGui_ImplGlfw_InitForOpenGL(m_window, false);
        ImGui_ImplOpenGL3_Init(nullptr);
        ImGui::StyleColorsDark();

        setFocused();
        setPosition(xpos, ypos);
        m_closed = false;
    }

    void Window::createWindow(const std::string& title, int width, int height, int xpos, int ypos, int ImGuiFlags) {
        if(m_window != nullptr) {
            throw exception("Window already initalized.", errorCodes::PNT_ERROR);
        }

        createWindowIntern(title, width, height, xpos, ypos, ImGuiFlags);
    }

    void Window::createWindow(const windowData& data) {
        if(m_window != nullptr) {
            throw exception("Window already initalized.", errorCodes::PNT_ERROR);
        }

        createWindowIntern(data.title.c_str(), data.width, data.height, data.xpos, data.ypos, data.ImGuiFlags);
        setEventCallback(data.eventCallback);
        if(data.focused) {
            setFocused();
        }
        if(data.hidden) {
            hide();
        } else {
            show();
        }
        if(data.iconified) {
            minimize();
        } else {
            maximize();
        }
        setVsyncMode(data.vsyncMode);
        setClearColor(data.clearColor[0], data.clearColor[1], data.clearColor[2], data.clearColor[3]);
        setUserPointer(data.userPointer);
    }

    void Window::destroyWindow() {
        if(!m_closed) {
            logger.get()->info("[PNT]Destroying window \"{}\"", m_data.title);

            m_instances--;
            m_instancesList.erase(std::find(m_instancesList.begin(), m_instancesList.end(), this));

            glfwDestroyWindow(m_window);
            delete m_openglContext;

            m_window = nullptr;
            m_closed = true;
        }
    }

    void Window::startFrame() {
        newframe = std::chrono::steady_clock::now();

        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }
        if(m_frame) {
            throw exception("Newframe already called.", errorCodes::PNT_ERROR);
        }

        glfwMakeContextCurrent(m_window);
        ImGui::SetCurrentContext(m_ImContext);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        m_frame = true;
    }

    void Window::endFrame() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }
        if(!m_frame) {
            throw exception("Endframe already called.", errorCodes::PNT_ERROR);
        }

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_openglContext->Viewport(0, 0, width, height);
        m_openglContext->ClearColor(m_data.clearColor[0], m_data.clearColor[1], m_data.clearColor[2], m_data.clearColor[3]);
        m_openglContext->Clear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(m_window);
        m_frame = false;

        endframe = std::chrono::steady_clock::now();
        deltaTime = endframe - newframe;
    }

    void Window::setEventCallback(void(*newEventCallback)(Window*, windowEvent)) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        m_data.eventCallback = newEventCallback;
    }

    void Window::pushEvent(windowEvent event) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        logger.get()->debug("[PNT]Pushing event of type \"{}\" for window \"{}\"", event.getTypename(), m_data.title);

        m_eventQueue.emplace_back(event);
    }

    void Window::setUserPointer(void* pointer) {
        m_data.userPointer = pointer;
    }

    void Window::setWindowData(windowData newData) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        m_data.eventCallback = newData.eventCallback;
        setTitle(newData.title);
        setDimentions(newData.width, newData.height);
        setPosition(newData.xpos, newData.ypos);
        setVsyncMode(newData.vsyncMode);
        newData.hidden ? show() : hide();
        newData.iconified ? minimize() : maximize();
        setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
    }

    void Window::setTitle(const std::string& title) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        this->m_data.title = title;
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void Window::setIcon(const GLFWimage& icon) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        logger.get()->debug("[PNT]Setting icon for window \"{}\"", m_data.title);

        if(icon.pixels != nullptr) {
            glfwSetWindowIcon(m_window, 1, &icon);
        } else {
            logger.get()->warn("[PNT]Icon failed to set");
            glfwSetWindowIcon(m_window, 0, nullptr);
        }
    }

    void Window::setDimentions(int width, int height) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwSetWindowSize(m_window, width, height);
        callbackManagers::windowsizeCallbackManager(m_window, width, height);
    }

    void Window::setFocused() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwFocusWindow(m_window);
        callbackManagers::windowFocusCallback(m_window, 1);
    }

    void Window::setPosition(int xpos, int ypos) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }
        if((xpos == GLFW_DONT_CARE) and (ypos == GLFW_DONT_CARE)) {
            return;
        }

        glfwSetWindowPos(m_window, xpos, ypos);
        callbackManagers::windowposCallbackManager(m_window, xpos, ypos);
    }

    void Window::hide() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwHideWindow(m_window);
    }

    void Window::show() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwShowWindow(m_window);
    }

    void Window::minimize() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwIconifyWindow(m_window);
        callbackManagers::iconifyCallbackManager(m_window, 1);
    }

    void Window::maximize() {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwRestoreWindow(m_window);
        callbackManagers::iconifyCallbackManager(m_window, 0);
    }

    void Window::setVsyncMode(vsyncModes vsyncMode) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        m_data.vsyncMode = vsyncMode;
        glfwSwapInterval((int)vsyncMode);
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        m_data.clearColor[0] = red;
        m_data.clearColor[1] = green;
        m_data.clearColor[2] = blue;
        m_data.clearColor[3] = alpha;
    }

    void Window::setShouldClose(bool shouldClose) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        glfwSetWindowShouldClose(m_window, shouldClose);
    }

    void Window::setAspectRatio(int numerator, int denominator) {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        logger.get()->debug("[PNT]Setting aspect ratio: {}, {} for window \"{}\"", numerator, denominator,m_data.title);

        glfwSetWindowAspectRatio(m_window, numerator, denominator);
    }

    std::chrono::duration<double> Window::getDeltaTime() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return deltaTime;
    }

    void* Window::getUserPointer() const {
        return m_data.userPointer;
    }

    windowData Window::getWindowData() const {
        return m_data;
    }

    std::string Window::getTitle() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.title;
    }

    int Window::getWidth() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.width;
    }

    int Window::getHeight() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.height;
    }

    bool Window::getFocus() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.focused;
    }

    int Window::getXPos() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.xpos;
    }

    int Window::getYPos() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.ypos;
    }

    bool Window::getHidden() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.hidden;
    }

    bool Window::getIconified() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_data.iconified;
    }

    const GladGLContext* Window::getGL() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return m_openglContext;
    }

    bool Window::shouldClose() const {
        if(m_window == nullptr) {
            throw exception("Window not initalized.", errorCodes::PNT_ERROR);
        }

        return glfwWindowShouldClose(m_window);
    }

    const GLFWwindow* Window::getGLFWWindow() const {
        return m_window;
    }

    // Callback definitions.

    void monitorCallback(GLFWmonitor* monitor, int event) {
        ImGui_ImplGlfw_MonitorCallback(monitor, event);
    }

    void callbackManagers::keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_KeyCallback(glfwWindow, key, scancode, action, mods);
        ImGui::SetCurrentContext(oldImContext);

        if(!window->m_IO->WantCaptureKeyboard && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createKeyEvent(key, scancode, action, mods));
        }
    }

    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_CharCallback(glfwWindow, codepoint);
        ImGui::SetCurrentContext(oldImContext);

        if(!window->m_IO->WantCaptureKeyboard && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createCharEvent(codepoint));
        }
    }

    void callbackManagers::dropCallbackManager(GLFWwindow* glfwWindow, int path_count, const char** paths) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createDropEvent(path_count, paths));
        }
    }

    void callbackManagers::scrollCallbackManager(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_ScrollCallback(glfwWindow, xoffset, yoffset);
        ImGui::SetCurrentContext(oldImContext);

        if(!window->m_IO->WantCaptureMouse && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createScrollEvent(xoffset, yoffset));
        }
    }

    void callbackManagers::cursorPosCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_CursorPosCallback(glfwWindow, xpos, ypos);
        ImGui::SetCurrentContext(oldImContext);

        if(!window->m_IO->WantCaptureMouse && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createCursorposEvent(xpos, ypos));
        }
    }

    void callbackManagers::windowposCallbackManager(GLFWwindow* glfwWindow, int xpos, int ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_data.xpos = xpos;
        window->m_data.ypos = ypos;
        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createWindowposEvent(xpos, ypos));
        }
    }

    void callbackManagers::windowsizeCallbackManager(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_data.width = width;
        window->m_data.height = height;
        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createWindowsizeEvent(width, height));
        }
    }

    void callbackManagers::cursorEnterCallback(GLFWwindow* glfwWindow, int entered) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_CursorEnterCallback(glfwWindow, entered);
        ImGui::SetCurrentContext(oldImContext);

        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createCursorEnterEvent(entered));
        }
    }

    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_MouseButtonCallback(glfwWindow, button, action, mods);
        ImGui::SetCurrentContext(oldImContext);

        if(!window->m_IO->WantCaptureMouse && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createMousebuttonEvent(button, action, mods));
        }
    }

    void callbackManagers::windowFocusCallback(GLFWwindow* glfwWindow, int focused) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_data.focused = focused;

        ImGuiContext* oldImContext = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(window->m_ImContext);
        ImGui_ImplGlfw_WindowFocusCallback(glfwWindow, focused);
        ImGui::SetCurrentContext(oldImContext);

        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createWindowFocusEvent(focused));
        }
    }

    void callbackManagers::iconifyCallbackManager(GLFWwindow* glfwWindow, int iconified) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_data.iconified = iconified;
        if(window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createIconifyEvent(iconified));
        }
    }
}
