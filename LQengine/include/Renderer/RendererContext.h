#pragma once
#include <string>
#include <utility>

#include "Core/Handle.h"
#include "Core/Types.h"

namespace LQ {
    LQ_HANDLE(DynamicIndexBufferHandle)
	LQ_HANDLE(DynamicVertexBufferHandle)
	LQ_HANDLE(FrameBufferHandle)
	LQ_HANDLE(IndexBufferHandle)
	LQ_HANDLE(IndirectBufferHandle)
	LQ_HANDLE(OcclusionQueryHandle)
	LQ_HANDLE(ProgramHandle)
	LQ_HANDLE(ShaderHandle)
	LQ_HANDLE(TextureHandle)
	LQ_HANDLE(UniformHandle)
	LQ_HANDLE(VertexBufferHandle)
	LQ_HANDLE(VertexLayoutHandle)

    class RendererContext : public Handle<RendererContext> {
        public:
            RendererContext() = default;
            ~RendererContext();

            RendererContext(const RendererContext&) = delete;
            auto operator=(const RendererContext&) -> RendererContext& = delete;

            RendererContext(RendererContext&& other) noexcept
                : Handle<RendererContext>(std::exchange(other.impl, nullptr)) {}
            auto operator=(RendererContext&& other) noexcept -> RendererContext&;

            auto init(this RendererContext& self) -> void;
            auto deinit(this RendererContext& self) -> void;
   
            auto new_frame(this RendererContext& self) -> void;
            auto end_frame(this RendererContext& self) -> void;

            auto handle_resize(this RendererContext& self) -> void;

            auto create_framebuffer(this RendererContext& self, Size size) -> FrameBufferHandle;

            auto create_shader(this RendererContext& self, const std::string& data, const std::string& name) -> ShaderHandle;
            auto create_shader(this RendererContext& self, const std::string& file_path) -> ShaderHandle;
    };
}