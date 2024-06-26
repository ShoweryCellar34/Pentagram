#pragma once

#include "includes.hpp"

namespace PNT {
    /// @brief Processes all pending events.
    void processEvents() {
        glfwPollEvents();
    }

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
        int path_count;
        char** paths;

        void setData(int path_count, char** paths) {
            init = true;
            this->paths = new char*[path_count];
            for(size_t i = 0; i < path_count; i++) {
                this->paths[i] = new char[strlen(paths[i]) + 1];
                strcpy(this->paths[i], paths[i]);
            }
        }

        dropEvent() {
            init = false;
        }
        ~dropEvent() {
            if(init) {
                for(size_t i = 0; i < path_count; i++) {
                    delete[] this->paths[i];
                }
                delete[] paths;
            }
        }

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

    windowEvent createKeyEvent(int key, int scancode, int action, int mods) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_KEYBOARD;
        event.keyboardEvent.key = key;
        event.keyboardEvent.scancode = scancode;
        event.keyboardEvent.action = action;
        event.keyboardEvent.mods = mods;

        return event;
    }
    windowEvent createCharEvent(unsigned int codepoint) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_CHAR;
        event.charEvent.codepoint = codepoint;

        return event;
    }
    windowEvent createDropEvent(int path_count, const char* paths[]) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_DROP;
        event.dropEvent.path_count = path_count;
        event.dropEvent.setData(path_count, (char**)paths);

        return event;
    }
    windowEvent createScrollEvent(double xoffset, double yoffset) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_SCROLL;
        event.scrollEvent.xoffset = xoffset;
        event.scrollEvent.yoffset = yoffset;

        return event;
    }
    windowEvent createCursorposEvent(double xpos, double ypos) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_CURSORPOS;
        event.cursorposEvent.xpos = xpos;
        event.cursorposEvent.ypos = ypos;

        return event;
    }
    windowEvent createWindowposEvent(int xpos, int ypos) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_WINDOWPOS;
        event.windowposEvent.xpos = xpos;
        event.windowposEvent.ypos = ypos;

        return event;
    }
    windowEvent createWindowsizeEvent(int width, int height) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_WINDOWSIZE;
        event.windowsizeEvent.width = width;
        event.windowsizeEvent.height = height;

        return event;
    }
    windowEvent createMousebuttonEvent(int button, int action, int mods) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_MOUSEBUTTON;
        event.mousebuttonEvent.button = button;
        event.mousebuttonEvent.action = action;
        event.mousebuttonEvent.mods = mods;

        return event;
    }
    windowEvent createIconifyEvent(bool iconified) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_ICONIFY;
        event.iconified = iconified;

        return event;
    }
}
