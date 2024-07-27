#pragma once

#include <spdlog/spdlog.h>

#define PNT_REQUIRE_INIT() if(!initialized) PNT::assertMsg(__FILE__, __LINE__, 0)
#define PNT_REQUIRE_DEINIT() if(initialized) PNT::assertMsg(__FILE__, __LINE__, 1)

#define PNT_WINDOW_ASSERT(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 2)
#define PNT_NO_WINDOW_ASSERT(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 3)
#define PNT_NEWFRAME_ASSERT(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 4)
#define PNT_ENDFRAME_ASSERT(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 5)

#define PNT_SHADER_ID(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 6)
#define PNT_NO_SHADER_ID(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 7)

#define PNT_PROGRAM_ID(x) if(!x) PNT::assertMsg(__FILE__, __LINE__, 8)
#define PNT_NO_PROGRAM_ID(x) if(x) PNT::assertMsg(__FILE__, __LINE__, 9)

namespace PNT {
    extern bool initialized;

    void assertMsg(const char* file, int line, int code);

    void errorCallback(int errorCode, const char* errorDescription);
}
