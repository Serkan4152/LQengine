#pragma once
#include <expected>
#include <string>
#include "Renderer/RendererContext.h"

namespace LQ {
    class Renderer {
    public:
        constexpr static auto MODULE_NAME = "Renderer";

        auto init(this Renderer& self) -> std::expected<void, std::string>;
        auto deinit(this Renderer& self) -> std::expected<void, std::string>;
        auto update(this Renderer& self) -> void;
    private:
        RendererContext* m_context{nullptr};
    };
}