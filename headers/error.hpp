#pragma once

#define PNT_WINDOW_ASSERT(x) if(!x) PNT::requireWindow(__FILE__, __LINE__)
#define PNT_NO_WINDOW_ASSERT(x) if(x) PNT::requireNoWindow(__FILE__, __LINE__)
#define PNT_NEWFRAME_ASSERT(x) if(!x) PNT::requireNewfrme(__FILE__, __LINE__)
#define PNT_ENDFRAME_ASSERT(x) if(x) PNT::requireEndfrme(__FILE__, __LINE__)

namespace PNT {
    void requireWindow(const char* file, int line);

    void requireNoWindow(const char* file, int line);

    void requireNewfrme(const char* file, int line);

    void requireEndfrme(const char* file, int line);

    void errorCallback(int errorCode, const char* errorDescription);
}
