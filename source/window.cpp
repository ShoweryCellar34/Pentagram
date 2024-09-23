#include <window.hpp>

#include <algorithm>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <error.hpp>
#include <event.hpp>

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    // Window definitions.

    Window::Window() : m_window(nullptr), m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), m_IO(nullptr) {
    }

    Window::Window(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) : m_window(nullptr),
         m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), m_IO(nullptr) {
        createWindow(title, width, height, xpos, ypos, ImGuiFlags);
    }

    Window::Window(const windowData& data) : m_window(nullptr), m_closed(true), m_frame(false), m_data(), m_eventQueue(), m_ImContext(nullptr), 
        m_IO(nullptr) {
        createWindow(data);
    }

    Window::~Window() {
        destroyWindow();
    }

    GladGLContext gl;
    void Window::createWindowIntern(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        PNT_REQUIRE_INIT();

        m_openglContext = &gl;

        logger.get()->info("[PNT]Creating window \"{}\"", title);

        m_instancesList.push_back(this);
        m_instances++;

        this->m_data.title = title;
        m_data.width = width;
        m_data.height = height;

        // Intel integrted graphics no likie-like these lines
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwSetWindowUserPointer(m_window, this);
        glfwMakeContextCurrent(m_window);
        gladLoadGLContext(m_openglContext, (GLADloadfunc)glfwGetProcAddress);
        setPosition(xpos, ypos);

        glfwSetKeyCallback(m_window, callbackManagers::keyCallbackManager);
        glfwSetCharCallback(m_window, callbackManagers::charCallbackManager);
        glfwSetDropCallback(m_window, callbackManagers::dropCallbackManager);
        glfwSetScrollCallback(m_window, callbackManagers::scrollCallbackManager);
        //glfwSetMonitorCallback(callbackManagers::);
        //glfwSetCharModsCallback(window, callbackManagers::);
        //glfwSetJoystickCallback(callbackManagers::);
        glfwSetCursorPosCallback(m_window, callbackManagers::cursorPosCallbackManager);
        glfwSetWindowPosCallback(m_window, callbackManagers::windowposCallbackManager);
        glfwSetWindowSizeCallback(m_window, callbackManagers::windowsizeCallbackManager);
        //glfwSetCursorEnterCallback(window, callbackManagers::);
        glfwSetMouseButtonCallback(m_window, callbackManagers::mousebuttonCallbackManager);
        //glfwSetWindowCloseCallback(window, callbackManagers::);
        //glfwSetWindowFocusCallback(window, callbackManagers::);
        glfwSetWindowIconifyCallback(m_window, callbackManagers::iconifyCallbackManager);
        //glfwSetWindowRefreshCallback(window, callbackManagers::);
        //glfwSetWindowMaximizeCallback(window, callbackManagers::);
        //glfwSetFramebufferSizeCallback(window, callbackManagers::);
        //glfwSetWindowContentScaleCallback(window, callbackManagers::);

        m_ImContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(m_ImContext);
        m_IO = &ImGui::GetIO();
        m_IO->ConfigFlags |= ImGuiFlags;
        ImGui_ImplGlfw_InitForOpenGL(m_window, false);
        ImGui_ImplOpenGL3_Init(nullptr);
        ImGui::StyleColorsDark();
        m_closed = false;
    }

    void Window::createWindow(const char *title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos, uint32_t ImGuiFlags) {
        PNT_NO_WINDOW_ASSERT(m_window);

        createWindowIntern(title, width, height, xpos, ypos, ImGuiFlags);
    }

    void Window::createWindow(const windowData& data) {
        PNT_NO_WINDOW_ASSERT(m_window);

        createWindowIntern(data.title.c_str(), data.width, data.height, data.xpos, data.ypos, data.ImGuiFlags);
    }

    void Window::destroyWindow() {
        if(!m_closed) {
            logger.get()->info("[PNT]Destroying window \"{}\"", m_data.title);

            m_instances--;
            m_instancesList.erase(std::find(m_instancesList.begin(), m_instancesList.end(), this));

            glfwDestroyWindow(m_window);

            m_window = nullptr;
            m_closed = true;
        }
    }

    void Window::startFrame() {
        newframe = std::chrono::steady_clock::now();

        PNT_WINDOW_ASSERT(m_window);
        PNT_ENDFRAME_ASSERT(m_frame);

        glfwMakeContextCurrent(m_window);
        ImGui::SetCurrentContext(m_ImContext);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        m_frame = true;
    }

    void Window::endFrame() {
        PNT_WINDOW_ASSERT(m_window);
        PNT_NEWFRAME_ASSERT(m_frame);

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
        PNT_WINDOW_ASSERT(m_window);

        m_data.eventCallback = newEventCallback;
    }

    void Window::pushEvent(windowEvent event) {
        PNT_WINDOW_ASSERT(m_window);

        logger.get()->debug("[PNT]Pushing event of type \"{}\" for window \"{}\"", event.getTypename(), m_data.title);

        m_eventQueue.push_back(event);
    }

    void Window::setUserPointer(void* pointer) {
        m_data.userPointer = pointer;
    }

    void Window::setWindowData(windowData newData) {
        PNT_WINDOW_ASSERT(m_window);

        m_data.eventCallback = newData.eventCallback;
        setTitle(newData.title);
        setDimentions(newData.width, newData.height);
        setPosition(newData.xpos, newData.ypos);
        setVsyncMode(newData.vsyncMode);
        newData.hidden ? show() : hide();
        newData.iconified ? minimize() : maximize();
        setClearColor(newData.clearColor[0], newData.clearColor[1], newData.clearColor[2], newData.clearColor[3]);
    }

    void Window::setTitle(const char* title) {
        PNT_WINDOW_ASSERT(m_window);

        this->m_data.title = title;
        glfwSetWindowTitle(m_window, title);
    }

    void Window::setTitle(std::string title) {
        PNT_WINDOW_ASSERT(m_window);

        this->m_data.title = title;
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void Window::setIcon(const GLFWimage& icon) {
        PNT_WINDOW_ASSERT(m_window);

        logger.get()->debug("[PNT]Setting icon for window \"{}\"", m_data.title);

        if(icon.pixels != nullptr) {
            glfwSetWindowIcon(m_window, 1, &icon);
        } else {
            logger.get()->warn("[PNT]Icon failed to set");
            glfwSetWindowIcon(m_window, 0, nullptr);
        }
    }

    void Window::setDimentions(uint16_t width, uint16_t height) {
        PNT_WINDOW_ASSERT(m_window);

        glfwSetWindowSize(m_window, width, height);
    }

    void Window::setPosition(uint16_t xpos, uint16_t ypos) {
        PNT_WINDOW_ASSERT(m_window);

        glfwSetWindowPos(m_window, xpos, ypos);
        callbackManagers::windowposCallbackManager(m_window, xpos, ypos);
    }

    void Window::hide() {
        PNT_WINDOW_ASSERT(m_window);

        glfwHideWindow(m_window);
    }

    void Window::show() {
        PNT_WINDOW_ASSERT(m_window);

        glfwShowWindow(m_window);
    }

    void Window::minimize() {
        PNT_WINDOW_ASSERT(m_window);

        glfwIconifyWindow(m_window);
    }

    void Window::maximize() {
        PNT_WINDOW_ASSERT(m_window);

        glfwRestoreWindow(m_window);
    }

    void Window::setVsyncMode(int8_t vsyncMode) {
        PNT_WINDOW_ASSERT(m_window);

        m_data.vsyncMode = vsyncMode;
        glfwSwapInterval(vsyncMode);
    }

    void Window::setClearColor(float red, float green, float blue, float alpha) {
        PNT_WINDOW_ASSERT(m_window);

        m_data.clearColor[0] = red;
        m_data.clearColor[1] = green;
        m_data.clearColor[2] = blue;
        m_data.clearColor[3] = alpha;
    }

    void Window::setShouldClose(bool shouldClose) {
        PNT_WINDOW_ASSERT(m_window);

        glfwSetWindowShouldClose(m_window, shouldClose);
    }

    void Window::setAspectRatio(uint32_t numerator, uint32_t denominator) {
        PNT_WINDOW_ASSERT(m_window);

        logger.get()->debug("[PNT]Setting aspect ratio: {}, {} for window \"{}\"", numerator, denominator,m_data.title);

        glfwSetWindowAspectRatio(m_window, numerator, denominator);
    }

    std::chrono::duration<double> Window::getDeltaTime() const {
        return deltaTime;
    }

    void* Window::getUserPointer() const {
        return m_data.userPointer;
    }

    std::string Window::getTitle() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.title;
    }

    uint16_t Window::getWidth() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.width;
    }

    uint16_t Window::getHeight() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.height;
    }

    uint16_t Window::getXPos() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.xpos;
    }

    uint16_t Window::getYPos() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.ypos;
    }

    bool Window::getHidden() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.hidden;
    }

    bool Window::getIconified() const {
        PNT_WINDOW_ASSERT(m_window);

        return m_data.iconified;
    }

    GladGLContext* Window::getGL() {
        PNT_WINDOW_ASSERT(m_window);

        return m_openglContext;
    }

    bool Window::shouldClose() const {
        PNT_WINDOW_ASSERT(m_window);

        return glfwWindowShouldClose(m_window);
    }

    // Callback definitions.

    void callbackManagers::keyCallbackManager(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->m_IO->WantCaptureKeyboard && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createKeyEvent(key, scancode, action, mods));
        }
    }

    void callbackManagers::charCallbackManager(GLFWwindow* glfwWindow, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
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
        if(!window->m_IO->WantCaptureMouse && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createScrollEvent(xoffset, yoffset));
        }
    }

    void callbackManagers::cursorPosCallbackManager(GLFWwindow* glfwWindow, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
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

    void callbackManagers::mousebuttonCallbackManager(GLFWwindow* glfwWindow, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        if(!window->m_IO->WantCaptureMouse && window->m_data.eventCallback != nullptr) {
            window->m_data.eventCallback(window, createMousebuttonEvent(button, action, mods));
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
