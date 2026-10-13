#pragma once
#include <expected>
#include <string>

namespace LQ{
    class EventSystem {
        public:
            auto init() -> std::expected<void, std::string>;
            auto deinit() -> std::expected<void, std::string>;
        private:
    };
}