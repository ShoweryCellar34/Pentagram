#include <event.hpp>

#include <cstring>
#include <string>
#include <GLFW/glfw3.h>
#include <window.hpp>
#include <enumerations.hpp>

namespace PNT {
    // Event definitions.

    void processEvents() {
        for(Window* window : Window::m_instancesList) {
            while(window->m_eventQueue.size()) {
                window->m_data.eventCallback(window, window->m_eventQueue.back());
                window->m_eventQueue.pop_back();
            }
        }
        glfwPollEvents();
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
        for(size_t i = 0; i < pathCount; i++) {
            event.dropFiles.paths.push_back(paths[i]);
        }

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
    const char *windowEvent::getTypename() {
        switch(type) {
        case PNT_EVENT_TYPE_CHAR:
            return "Text input";

        case PNT_EVENT_TYPE_CURSORPOS:
            return "Cursor position";

        case PNT_EVENT_TYPE_DROP:
            return "File(s) droped";

        case PNT_EVENT_TYPE_ICONIFY:
            return "Minimized/Maximized";

        case PNT_EVENT_TYPE_KEYBOARD:
            return "Key press";

        case PNT_EVENT_TYPE_MOUSEBUTTON:
            return "Mouse button";

        case PNT_EVENT_TYPE_SCROLL:
            return "Scroll";

        case PNT_EVENT_TYPE_WINDOWPOS:
            return "Window position";

        case PNT_EVENT_TYPE_WINDOWSIZE:
            return "Window size";

        default:
            return "Unregistered event name";
        }
    }
}
