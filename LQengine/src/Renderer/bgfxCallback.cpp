#include "bgfxCallback.h"
#include "bgfx/bgfx.h"
#include "spdlog/spdlog.h"

#include <array>
#include <cstdarg>
#include <cstdio>
#include "Utils/Log.h"

namespace LQ {
    bgfxCallback::bgfxCallback()
    {

    }

    void bgfxCallback::fatal(
    const char* file,
    uint16_t line,
    bgfx::Fatal::Enum code,
    const char* str
    ) {
        auto logger = LQ::Log::get_bgfx_logger();

        logger->log(
            spdlog::source_loc{
                file ? file : "",
                static_cast<int>(line),
                ""
            },
            spdlog::level::critical,
            "0x{:08X}: {}",
            static_cast<uint32_t>(code),
            str ? str : "<no message>"
        );
    }

    void bgfxCallback::traceVargs(
        const char* file,
        uint16_t line,
        const char* format,
        va_list args
    ) {
        std::array<char, 4096> buffer{}; 

        if (format) { 
            std::vsnprintf(buffer.data(), buffer.size(), format, args); 
        } 

        for (char* p = buffer.data(); *p != '\0'; ++p) { 
            if (*p == '\n' || *p == '\r') { 
                *p = ' '; 
            } else if (*p == '\t') {
                *p = ' '; 
            } 
        }
        
        auto logger = LQ::Log::get_bgfx_logger();

        logger->log(
            spdlog::source_loc{
                file ? file : "",
                static_cast<int>(line),
                ""
            },
            spdlog::level::debug,
            "{}",
            buffer.data()
        );
    }
    
    void bgfxCallback::profilerBegin(const char *, uint32_t, const char *, uint16_t)
    {
    }

    void bgfxCallback::profilerBeginLiteral(const char *, uint32_t, const char *, uint16_t)
    {
    }

    void bgfxCallback::profilerEnd()
    {
    }

    uint32_t bgfxCallback::cacheReadSize(uint64_t)
    {
        return 0;
    }

    bool bgfxCallback::cacheRead(uint64_t, void *, uint32_t)
    {
        return false;
    }

    void bgfxCallback::cacheWrite(uint64_t, const void *, uint32_t)
    {
    }

    void bgfxCallback::screenShot(const char *, uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, const void *, uint32_t, bool)
    {
    }

    void bgfxCallback::captureBegin(uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, bool)
    {
    }

    void bgfxCallback::captureEnd()
    {
    }

    void bgfxCallback::captureFrame(const void *, uint32_t)
    {
    }
}