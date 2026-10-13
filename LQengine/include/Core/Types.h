#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <typeindex>

namespace LQ {
    struct Size
    {
        int width{0};
        int height{0};
    };
    
    struct TypeIndexHash {
        std::size_t operator()(const std::type_index& ti) const noexcept { return ti.hash_code(); }
    };
}