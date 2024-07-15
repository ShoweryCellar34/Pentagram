#pragma once

#include <vector>
#include <stdint.h>

namespace PNT {
    class Window;
    struct windowEvent;
    static std::vector<std::pair<Window*, windowEvent>> customEventQueue;

    /// @brief Processes all pending events.
    void processEvents();

    /// @brief Pushes an event to the event stack.
    /// @param window The desired "PNT::Window*" to associate the event with.
    /// @param event The desired event to push, you can create events with the numerous "create...Event(...);" functions.
    /// @warning glfw has no event queue manipulation that I know of, so all custom events push by this function will be proccesed before glfw events.
    void pushEvent(Window* window, windowEvent event);

    struct keyEvent {
        int key;
        int scancode;
        int action;
        int mods;
    };

    struct charEvent {
        unsigned int codepoint;
    };

    struct dropEvent {
        size_t pathCount;
        char** paths;

        void setData(size_t pathCount, char** paths);

        dropEvent();

        ~dropEvent();

    private:
        bool init = false;
    };

    struct scrollEvent {
        double xoffset;
        double yoffset;
    };

    struct cursorposEvent {
        double xpos;
        double ypos;
    };

    struct windowposEvent {
        int xpos;
        int ypos;
    };

    struct windowsizeEvent {
        int width;
        int height;
    };

    struct mousebuttonEvent {
        int button;
        int action;
        int mods;
    };

    // Structure for events.
    struct windowEvent {
        unsigned short type;
        keyEvent keyboard;
        charEvent character;
        dropEvent dropFiles;
        scrollEvent scroll;
        cursorposEvent cursorpos;
        windowposEvent windowpos;
        windowsizeEvent windowsize;
        mousebuttonEvent mousebutton;
        bool iconified;
    };

    windowEvent createKeyEvent(int key, int scancode, int action, int mods);
    windowEvent createCharEvent(unsigned int codepoint);
    windowEvent createDropEvent(size_t pathCount, const char* paths[]);
    windowEvent createScrollEvent(double xoffset, double yoffset);
    windowEvent createCursorposEvent(double xpos, double ypos);
    windowEvent createWindowposEvent(int xpos, int ypos);
    windowEvent createWindowsizeEvent(int width, int height);
    windowEvent createMousebuttonEvent(int button, int action, int mods);
    windowEvent createIconifyEvent(bool iconified);
}
