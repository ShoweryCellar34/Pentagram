#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <stddef.h>

namespace PNT {
    class Window;
    struct windowEvent;

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
        std::vector<std::string> paths;
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

    struct cursorEnterEvent {
        int entered;
    };

    struct mousebuttonEvent {
        int button;
        int action;
        int mods;
    };

    struct windowfocusEvent {
        int focused;
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
        cursorEnterEvent cursorenter;
        mousebuttonEvent mousebutton;
        windowfocusEvent windowfocus;
        bool iconified;

        const char* getTypename();
    };

    windowEvent createKeyEvent(int key, int scancode, int action, int mods);
    windowEvent createCharEvent(unsigned int codepoint);
    windowEvent createDropEvent(size_t pathCount, const char* paths[]);
    windowEvent createScrollEvent(double xoffset, double yoffset);
    windowEvent createCursorposEvent(double xpos, double ypos);
    windowEvent createWindowposEvent(int xpos, int ypos);
    windowEvent createWindowsizeEvent(int width, int height);
    windowEvent createCursorEnterEvent(int entered);
    windowEvent createMousebuttonEvent(int button, int action, int mods);
    windowEvent createWindowFocusEvent(int focused);
    windowEvent createIconifyEvent(bool iconified);
}
