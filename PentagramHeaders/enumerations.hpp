#pragma once


#ifdef LOGGER
enum loggerFlags
{
    PNT_LOGGER_NOTE,
    PNT_LOGGER_WARNING,
    PNT_LOGGER_ERROR,
    PNT_LOGGER_FATAL
};
#endif

#ifdef VSYNC
enum vsyncFlags
{
    PNT_VSYNC_ADAPTIVE = -1,
    PNT_VSYNC_OFF,
    PNT_VSYNC_ON
};
#endif
