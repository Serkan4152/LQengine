#pragma once
#include "imgui.h"
#include <expected>
#include <string>

namespace LQ {
    class ImGuiRenderer {
        public:
            constexpr static auto MODULE_NAME = "ImGuiRenderer";

            ImGuiRenderer() = default;
            ~ImGuiRenderer() = default;

            auto init(this ImGuiRenderer& self) -> std::expected<void, std::string>;
            auto deinit(this ImGuiRenderer& self) -> std::expected<void, std::string>;

            auto begin_frame(this ImGuiRenderer& self) -> void;
            auto end_frame(this ImGuiRenderer& self) -> void;

            auto process_event(this ImGuiRenderer& self, const void* event) -> void;
        private:
    };
}