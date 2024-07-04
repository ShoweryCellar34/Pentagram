#include <event.hpp>

namespace PNT {
    void processEvents() {
        glfwPollEvents();
    }

    void dropEvent::setData(size_t pathCount, char** paths) {
        init = true;
        this->paths = new char*[pathCount];
        for(size_t i = 0; i < pathCount; i++) {
            this->paths[i] = new char[strlen(paths[i]) + 1];
            strcpy(this->paths[i], paths[i]);
        }
    }

    dropEvent::dropEvent() : init(false) {
    }

    dropEvent::dropEvent(const dropEvent& original) : init(original.init), pathCount(original.pathCount) {
        paths = new char*[original.pathCount];
        for(size_t i = 0; i < original.pathCount; i++) {
            paths[i] = new char[strlen(original.paths[i]) + 1];
            strcpy(paths[i], original.paths[i]);
        }
    }

    dropEvent::~dropEvent() {
        if(init) {
            for(size_t i = 0; i < pathCount; i++) {
                delete[] this->paths[i];
            }
            delete[] paths;
        }
    }

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

    windowEvent createDropEvent(size_t pathCount, const char* paths[]) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_DROP;
        event.dropEvent.pathCount = pathCount;
        event.dropEvent.setData(pathCount, (char**)paths);

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
