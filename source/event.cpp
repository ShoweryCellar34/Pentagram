#include <event.hpp>

#include <cstring>
#include <GLFW/glfw3.h>
#include <enumerations.hpp>

namespace PNT {
    // Event definitions.

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

    dropEvent::~dropEvent() {
        if(init) {
            for(size_t i = 0; i < pathCount; i++) {
                delete[] this->paths[i];
            }
            delete[] paths;
        }
    }

    // Event creation function definitions.

    windowEvent createKeyEvent(int key, int scancode, int action, int mods) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_KEYBOARD;
        event.keyboard.key = key;
        event.keyboard.scancode = scancode;
        event.keyboard.action = action;
        event.keyboard.mods = mods;

        return event;
    }

    windowEvent createCharEvent(unsigned int codepoint) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_CHAR;
        event.character.codepoint = codepoint;

        return event;
    }

    windowEvent createDropEvent(size_t pathCount, const char* paths[]) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_DROP;
        event.dropFiles.pathCount = pathCount;
        event.dropFiles.setData(pathCount, (char**)paths);

        return event;
    }

    windowEvent createScrollEvent(double xoffset, double yoffset) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_SCROLL;
        event.scroll.xoffset = xoffset;
        event.scroll.yoffset = yoffset;

        return event;
    }

    windowEvent createCursorposEvent(double xpos, double ypos) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_CURSORPOS;
        event.cursorpos.xpos = xpos;
        event.cursorpos.ypos = ypos;

        return event;
    }

    windowEvent createWindowposEvent(int xpos, int ypos) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_WINDOWPOS;
        event.windowpos.xpos = xpos;
        event.windowpos.ypos = ypos;

        return event;
    }

    windowEvent createWindowsizeEvent(int width, int height) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_WINDOWSIZE;
        event.windowsize.width = width;
        event.windowsize.height = height;

        return event;
    }

    windowEvent createMousebuttonEvent(int button, int action, int mods) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_MOUSEBUTTON;
        event.mousebutton.button = button;
        event.mousebutton.action = action;
        event.mousebutton.mods = mods;

        return event;
    }

    windowEvent createIconifyEvent(bool iconified) {
        windowEvent event;

        event.type = PNT_EVENT_TYPE_ICONIFY;
        event.iconified = iconified;

        return event;
    }
}
