#pragma once

namespace PNT
{
    #ifdef LOGGER
    enum loggerFlags
    {
        NOTE,
        WARNING,
        ERROR,
        FATAL
    };
    #endif

    #ifdef VSYNC
    enum vsyncFlags
    {
        ADAPTIVE = -1,
        OFF,
        ON
    };
    #endif
}
