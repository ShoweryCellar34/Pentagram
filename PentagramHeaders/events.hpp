#pragma once

#include <includes.hpp>

namespace PNT {
    struct keyboardEvent {
        int key;
        int scancode;
        int action;
        int mods;
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
        keyboardEvent keyboardEvent;
        scrollEvent scrollEvent;
        cursorposEvent cursorposEvent;
        mousebuttonEvent mousebuttonEvent;
    };
}
