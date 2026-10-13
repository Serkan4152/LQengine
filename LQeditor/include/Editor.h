#pragma once
#include <expected>
#include <string>

namespace LQ {
    class Editor {
    public:
        constexpr static auto MODULE_NAME = "Editor";

        auto init(this Editor& self) -> std::expected<void, std::string>;
        auto deinit(this Editor& self) -> std::expected<void, std::string>;
        auto update(this Editor& self) -> void;
    private:
    };
}
