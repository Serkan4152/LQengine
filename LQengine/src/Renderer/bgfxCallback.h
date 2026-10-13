#pragma once
#include "bgfx/bgfx.h"

namespace LQ {
    class bgfxCallback final : public bgfx::CallbackI
    {
    public:
        bgfxCallback();
        
        void fatal(
            const char* file,
            uint16_t line,
            bgfx::Fatal::Enum code,
            const char* str
        ) override;

        void traceVargs(
            const char* file,
            uint16_t line,
            const char* format,
            va_list args
        ) override;

        void profilerBegin(
            const char*,
            uint32_t,
            const char*,
            uint16_t
        ) override;

        void profilerBeginLiteral(
            const char*,
            uint32_t,
            const char*,
            uint16_t
        ) override;

        void profilerEnd() override;

        uint32_t cacheReadSize(uint64_t) override;

        bool cacheRead(
            uint64_t,
            void*,
            uint32_t
        ) override;

        void cacheWrite(
            uint64_t,
            const void*,
            uint32_t
        ) override;

        void screenShot(
            const char*,
            uint32_t,
            uint32_t,
            uint32_t,
            bgfx::TextureFormat::Enum,
            const void*,
            uint32_t,
            bool
        ) override;

        void captureBegin(
            uint32_t,
            uint32_t,
            uint32_t,
            bgfx::TextureFormat::Enum,
            bool
        ) override;

        void captureEnd() override;

        void captureFrame(
            const void*,
            uint32_t
        ) override;
    };
}