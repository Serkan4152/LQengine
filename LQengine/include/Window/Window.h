#pragma once
#include <cstdint>
#include <string>
#include <utility>
#include "Core/Handle.h"
#include "Core/Types.h"

namespace LQ{
    enum class WindowFlags : std::uint64_t {
        None             = 0,
        Fullscreen       = 0x0000000000000001,
        Hidden           = 0x0000000000000008,
        Borderless       = 0x0000000000000010,
        Resizable        = 0x0000000000000020,
        Minimized        = 0x0000000000000040,
        Maximized        = 0x0000000000000080,
        HighPixelDensity = 0x0000000000002000,
        AlwaysOnTop      = 0x0000000000010000
    };

    constexpr auto operator|(WindowFlags lhs, WindowFlags rhs) -> WindowFlags
    {
        return static_cast<WindowFlags>(
            static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs)
        );
    }

    constexpr auto operator&(WindowFlags lhs, WindowFlags rhs) -> WindowFlags
    {
        return static_cast<WindowFlags>(
            static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs)
        );
    }

    constexpr auto has_flag(WindowFlags flags, WindowFlags flag) -> bool
    {
        return (flags & flag) == flag;
    }

    struct WindowInfo{
        std::string title{};
        Size        size{};

        WindowFlags flags = WindowFlags::None;
    };

    class Window : public Handle<Window> {
        public:
            using Handle<Window>::Handle;

            Window() = default;
            ~Window();

            Window(const Window&) = delete;
            auto operator=(const Window&) -> Window& = delete;

            Window(Window&& other) noexcept
                : Handle<Window>(std::exchange(other.impl, nullptr)) {}
            auto operator=(Window&& other) noexcept -> Window&;

            static auto create(const WindowInfo& window_info) -> Window;

            auto update() const -> void;
            auto poll() const -> void;
            auto destroy() -> void;

            auto get_title() const -> std::string;
            auto get_size() const -> Size;
            auto get_size_in_pixels() const -> Size;
            auto get_flags() const -> WindowFlags;

            auto set_title(const std::string& title) const -> bool;
            auto set_size(Size size) const -> bool;
            auto set_resizable(bool resizable) const -> bool;
            auto get_handle() const -> void*;
        };
}