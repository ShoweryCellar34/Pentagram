#pragma once

#include <includes.hpp>

namespace PNT {
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
        const char* paths[0];

        void setData(int path_count, const char* paths[]) {
            delete paths;
            const char* newPaths = new const char* [path_count];
            for(int i; i < path_count; i++) {
                this->paths[i] = 
            }
        }
    };

    struct scrollEvent {
        double xoffset;
        double yoffset;
    };

    struct cursorposEvent {
        double xpos;
        double ypos;
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
        mousebuttonEvent mousebuttonEvent;
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
        event.dropEvent

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

    windowEvent createMousebuttonEvent(int button, int action, int mods) {
        windowEvent event;

        event.eventType = PNT_EVENT_TYPE_MOUSEBUTTON;
        event.mousebuttonEvent.button = button;
        event.mousebuttonEvent.action = action;
        event.mousebuttonEvent.mods = mods;

        return event;
    }
}
