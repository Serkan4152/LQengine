#include "Window/Window.h"
#include "Core/App.h"
#include "Renderer/Renderer.h"

#include "SDL3/SDL.h"

namespace LQ{
    template <>
    struct Handle<Window>::Impl {
        SDL_Window* handle = nullptr;
    };

    auto Window::create(const WindowInfo &window_info) -> Window
    {   
        auto* handle = SDL_CreateWindow(
            window_info.title.c_str(),
            window_info.size.width,
            window_info.size.height,
            static_cast<SDL_WindowFlags>(window_info.flags)
        );

        LQ_ASSERT(handle, "Failed to create window. Error Code: {}", SDL_GetError());
        return Window(new Impl{handle});
    }

    Window::~Window()
    {
        destroy();
    }

    auto Window::operator=(Window&& other) noexcept -> Window&
    {
        if (this != &other) {
            destroy();
            impl = std::exchange(other.impl, nullptr);
        }
        return *this;
    }

    auto Window::update() const -> void
    {
        poll();
    }

    auto Window::poll() const -> void
    {
        LQ_ASSERT(impl && impl->handle, "Cannot poll events for an invalid window.");

        SDL_Event e = {};
        while(SDL_PollEvent(&e)) {
            if(App::has_mod<ImGuiRenderer>()) {
                App::mod<ImGuiRenderer>().process_event(&e);
            }

            switch(e.type) {
                case SDL_EVENT_QUIT: {
                    App::get()->should_stop();
                }break;
                case SDL_EVENT_WINDOW_RESIZED:
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                    if (e.window.windowID == SDL_GetWindowID(impl->handle)
                        && App::has_mod<Renderer>()) {
                        App::get_rendercontext().handle_resize();
                    }
                }break;
                default:
                    break;
            }
        }
    }

    auto Window::destroy() -> void
    {
        if (!impl) {
            return;
        }

        if (impl->handle) {
            SDL_DestroyWindow(impl->handle);
            impl->handle = nullptr;
        }
        delete impl;
        impl = nullptr;
    }

    auto Window::get_title() const -> std::string
    {
        LQ_ASSERT(impl && impl->handle, "Cannot get the title of an invalid window.");
        const auto* title = SDL_GetWindowTitle(impl->handle);
        LQ_ASSERT(title, "Failed to get window title. Error Code: {}", SDL_GetError());
        return title;
    }

    auto Window::get_size() const -> Size
    {
        LQ_ASSERT(impl && impl->handle, "Cannot get the size of an invalid window.");

        Size size{};
        LQ_ASSERT(
            SDL_GetWindowSize(impl->handle, &size.width, &size.height),
            "Failed to get window size. Error Code: {}",
            SDL_GetError()
        );
        return size;
    }

    auto Window::get_size_in_pixels() const -> Size
    {
        LQ_ASSERT(impl && impl->handle, "Cannot get the pixel size of an invalid window.");

        Size size{};
        LQ_ASSERT(
            SDL_GetWindowSizeInPixels(impl->handle, &size.width, &size.height),
            "Failed to get window pixel size. Error Code: {}",
            SDL_GetError()
        );
        return size;
    }

    auto Window::get_flags() const -> WindowFlags
    {
        LQ_ASSERT(impl && impl->handle, "Cannot get the flags of an invalid window.");
        return static_cast<WindowFlags>(SDL_GetWindowFlags(impl->handle));
    }

    auto Window::set_title(const std::string& title) const -> bool
    {
        LQ_ASSERT(impl && impl->handle, "Cannot set the title of an invalid window.");
        return SDL_SetWindowTitle(impl->handle, title.c_str());
    }

    auto Window::set_size(Size size) const -> bool
    {
        LQ_ASSERT(impl && impl->handle, "Cannot set the size of an invalid window.");
        return SDL_SetWindowSize(impl->handle, size.width, size.height);
    }

    auto Window::set_resizable(bool resizable) const -> bool
    {
        LQ_ASSERT(impl && impl->handle, "Cannot change an invalid window.");
        return SDL_SetWindowResizable(impl->handle, resizable);
    }

    auto Window::get_handle() const -> void *
    {
        LQ_ASSERT(impl && impl->handle, "Cannot get the handle of an invalid window.");
        return impl->handle;
    }
}
