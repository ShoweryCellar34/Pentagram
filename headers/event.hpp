#pragma once

#include <cstddef>
#include <cstring>
#include <GLFW/glfw3.h>
#include <enumerations.hpp>

namespace PNT {
    /// @brief Processes all pending events.
    void processEvents();

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
        size_t path_count;
        char** paths;

        void setData(size_t path_count, char** paths);

        dropEvent();

        dropEvent(dropEvent&);

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
        unsigned short eventType;
        keyEvent keyboardEvent;
        charEvent charEvent;
        dropEvent dropEvent;
        scrollEvent scrollEvent;
        cursorposEvent cursorposEvent;
        windowposEvent windowposEvent;
        windowsizeEvent windowsizeEvent;
        mousebuttonEvent mousebuttonEvent;
        bool iconified;
    };

    windowEvent createKeyEvent(int key, int scancode, int action, int mods);
    windowEvent createCharEvent(unsigned int codepoint);
    windowEvent createDropEvent(size_t path_count, const char* paths[]);
    windowEvent createScrollEvent(double xoffset, double yoffset);
    windowEvent createCursorposEvent(double xpos, double ypos);
    windowEvent createWindowposEvent(int xpos, int ypos);
    windowEvent createWindowsizeEvent(int width, int height);
    windowEvent createMousebuttonEvent(int button, int action, int mods);
    windowEvent createIconifyEvent(bool iconified);
}
