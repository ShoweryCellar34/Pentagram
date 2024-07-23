#pragma once

#include <spdlog/spdlog.h>

#define PNT_WINDOW_ASSERT(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 0)
#define PNT_NO_WINDOW_ASSERT(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 1)
#define PNT_NEWFRAME_ASSERT(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 2)
#define PNT_ENDFRAME_ASSERT(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 3)

namespace PNT {
    extern std::shared_ptr<spdlog::logger> logger;

    void assertMsg(const char* file, int line, int code);

    void errorCallback(int errorCode, const char* errorDescription);
}
