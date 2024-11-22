#include <PNT/event.hpp>

#include <cstring>
#include <string>
#include <GLFW/glfw3.h>
#include <PNT/window.hpp>

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

        event.type = eventTypes::KEYBOARD;
        event.keyboard.key = key;
        event.keyboard.scancode = scancode;
        event.keyboard.action = action;
        event.keyboard.mods = mods;

        return event;
    }

    windowEvent createCharEvent(unsigned int codepoint) {
        windowEvent event;

        event.type = eventTypes::CHAR;
        event.character.codepoint = codepoint;

        return event;
    }

    windowEvent createDropEvent(int pathCount, const char* paths[]) {
        windowEvent event;

        event.type = eventTypes::DROP;
        for(int i = 0; i < pathCount; i++) {
            event.dropFiles.paths.emplace_back(paths[i]);
        }

        return event;
    }

    windowEvent createScrollEvent(double xoffset, double yoffset) {
        windowEvent event;

        event.type = eventTypes::SCROLL;
        event.scroll.xoffset = xoffset;
        event.scroll.yoffset = yoffset;

        return event;
    }

    windowEvent createCursorposEvent(double xpos, double ypos) {
        windowEvent event;

        event.type = eventTypes::CURSORPOS;
        event.cursorpos.xpos = xpos;
        event.cursorpos.ypos = ypos;

        return event;
    }

    windowEvent createWindowposEvent(int xpos, int ypos) {
        windowEvent event;

        event.type = eventTypes::WINDOWPOS;
        event.windowpos.xpos = xpos;
        event.windowpos.ypos = ypos;

        return event;
    }

    windowEvent createWindowsizeEvent(int width, int height) {
        windowEvent event;

        event.type = eventTypes::WINDOWSIZE;
        event.windowsize.width = width;
        event.windowsize.height = height;

        return event;
    }

    windowEvent createCursorEnterEvent(int entered) {
        windowEvent event;

        event.type = eventTypes::CURSORENTER;
        event.cursorenter.entered = entered;

        return event;
    }

    windowEvent createMousebuttonEvent(int button, int action, int mods) {
        windowEvent event;

        event.type = eventTypes::MOUSEBUTTON;
        event.mousebutton.button = button;
        event.mousebutton.action = action;
        event.mousebutton.mods = mods;

        return event;
    }

    windowEvent createWindowFocusEvent(int focused) {
        windowEvent event;

        event.type = eventTypes::WINDOWFOCUS;
        event.windowfocus.focused = focused;

        return event;
    }

    windowEvent createIconifyEvent(bool iconified) {
        windowEvent event;

        event.type = eventTypes::ICONIFY;
        event.iconified = iconified;

        return event;
    }
    const char *windowEvent::getTypename() {
        switch(type) {
            case eventTypes::KEYBOARD:
                return "Key press";
                break;
            case eventTypes::CHAR:
                return "Text input";
                break;
            case eventTypes::DROP:
                return "File(s) droped";
                break;
            case eventTypes::SCROLL:
                return "Scroll";
                break;
            case eventTypes::CURSORPOS:
                return "Cursor position";
                break;
            case eventTypes::WINDOWPOS:
                return "Window position";
                break;
            case eventTypes::WINDOWSIZE:
                return "Window size";
                break;
            case eventTypes::CURSORENTER:
                return "Coursor enter";
                break;
            case eventTypes::MOUSEBUTTON:
                return "Mouse button";
                break;
            case eventTypes::WINDOWFOCUS:
                return "WIndow focus";
                break;
            case eventTypes::ICONIFY:
                return "Minimized/Maximized";
                break;
            default:
                return "Unregistered event name";
                break;
        }
    }
}
