#include "Renderer/RendererContext.h"
#include "Core/App.h"
#include "Window/Window.h"

#include "bgfxCallback.h"

#include <cstdint>

#include "bgfx/bgfx.h"
#include "bx/bx.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_system.h"

namespace LQ {
    template <>
    struct Handle<RendererContext>::Impl {
        const bgfx::ViewId kClearView = 0;
        bgfxCallback m_callback;
        bool initialized = false;
    };

    RendererContext::~RendererContext()
    {
        deinit();
    }

    auto RendererContext::operator=(RendererContext&& other) noexcept -> RendererContext&
    {
        if (this != &other) {
            deinit();
            impl = std::exchange(other.impl, nullptr);
        }
        return *this;
    }

    auto RendererContext::init(this RendererContext &self) -> void
    {
        LQ_ASSERT(!self.impl, "Renderer context is already initialized.");
        self.impl = new Impl;
        self.impl->m_callback = bgfxCallback();

        const auto& window = App::get()->get_window();
        auto window_handle = (SDL_Window *)window.get_handle();
        auto size = window.get_size_in_pixels();
        
        bgfx::renderFrame(); 
        bgfx::Init init{};
        init.type = bgfx::RendererType::Vulkan;
        init.swapChain.width = size.width;
        init.swapChain.height = size.height;
        init.reset = BGFX_RESET_NONE; 
        init.callback = &self.impl->m_callback;
        
        const auto window_properties = SDL_GetWindowProperties(window_handle);
        const char *video_driver = SDL_GetCurrentVideoDriver();
        LQ_ASSERT(video_driver != nullptr, "Failed to get current SDL video driver. Error Code: {}", SDL_GetError());

        if (SDL_strcmp(video_driver, "windows") == 0) {
            init.swapChain.nwh = SDL_GetPointerProperty(window_properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
            LQ_ASSERT(init.swapChain.nwh != nullptr, "Failed to get HWND from SDL window. Error Code: {}", SDL_GetError());
        } else if (SDL_strcmp(video_driver, "cocoa") == 0) {
            init.swapChain.nwh = SDL_GetPointerProperty(window_properties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
            LQ_ASSERT(init.swapChain.nwh != nullptr, "Failed to get NSWindow from SDL window. Error Code: {}", SDL_GetError());
        } else if (SDL_strcmp(video_driver, "x11") == 0) {
            init.swapChain.ndt = SDL_GetPointerProperty(window_properties, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
            LQ_ASSERT(init.swapChain.ndt != nullptr, "Failed to get X11 display from SDL window. Error Code: {}", SDL_GetError());

            const auto window_id = SDL_GetNumberProperty(window_properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
            LQ_ASSERT(window_id != 0, "Failed to get X11 window ID from SDL window. Error Code: {}", SDL_GetError());
            init.swapChain.nwh = reinterpret_cast<void *>(static_cast<uintptr_t>(window_id));
        } else if (SDL_strcmp(video_driver, "wayland") == 0) {
            init.swapChain.ndt = SDL_GetPointerProperty(window_properties, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
            LQ_ASSERT(init.swapChain.ndt != nullptr, "Failed to get wl_display from SDL window. Error Code: {}", SDL_GetError());

            init.swapChain.nwh = SDL_GetPointerProperty(window_properties, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
            LQ_ASSERT(init.swapChain.nwh != nullptr, "Failed to get wl_surface from SDL window. Error Code: {}", SDL_GetError());
        } else {
            LQ_ASSERT(false, "Unsupported SDL video driver: {}", video_driver);
        }

        self.impl->initialized = bgfx::init(init);
        bgfx::setViewRect(self.impl->kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        bgfx::setViewClear(
            self.impl->kClearView,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x202030FF,
            1.0f,
            0
        );	
    }

    auto RendererContext::deinit(this RendererContext &self) -> void
    {
        if (!self.impl) {
            return;
        }

        if (self.impl->initialized) {
            bgfx::shutdown();
        }
        delete self.impl;
        self.impl = nullptr;
    }
    
    auto RendererContext::new_frame(this RendererContext &self) -> void
    {
        bgfx::touch(self.impl->kClearView);      

        if(App::has_mod<ImGuiRenderer>()) {
            App::mod<ImGuiRenderer>().begin_frame();
        }
    }

    auto RendererContext::end_frame(this RendererContext &self) -> void
    {
        if(App::has_mod<ImGuiRenderer>()) {
            App::mod<ImGuiRenderer>().end_frame();
        }

        bgfx::frame();
    }

    auto RendererContext::handle_resize(this RendererContext &self) -> void
    {
        auto size = App::get()->get_window().get_size_in_pixels();

        bgfx::SwapChain desc;
        desc.width = size.width;
        desc.height = size.height;

        bgfx::reset(0, &desc);
        bgfx::setViewRect(self.impl->kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    auto RendererContext::create_framebuffer(this RendererContext &self, Size size) -> FrameBufferHandle
    {
        bgfx::TextureHandle colorTexture = bgfx::createTexture2D(
            size.width, size.height,
            false, 1, 
            bgfx::TextureFormat::RGBA8, 
            BGFX_TEXTURE_RT
        );

        bgfx::TextureHandle depthTexture = bgfx::createTexture2D(
            size.width, size.height,
            false, 1, 
            bgfx::TextureFormat::D24S8, 
            BGFX_TEXTURE_RT
        );

        bgfx::TextureHandle textures[] = { colorTexture, depthTexture };
        bgfx::FrameBufferHandle fbo = bgfx::createFrameBuffer(
            BX_COUNTOF(textures), 
            textures, 
            true // fbo yok edildiğinde içerisindeki dokuları da otomatik siler
        );

        return FrameBufferHandle{fbo.idx};
    }

    auto RendererContext::create_shader(this RendererContext &self, const std::string &data, const std::string &name) -> ShaderHandle
    {
        const bgfx::ShaderHandle handle = bgfx::createShader(bgfx::copy(data.data(), data.size()));

        if(!bgfx::isValid(handle)) {
            LQ_LOG_ERROR("Failed to create shader '{}'.", name);
            return LQ_INVALID_HANDLE;
        } 

        bgfx::setName(handle, name.c_str());

        LQ_LOG_INFO("Successfully created shader '{}'.", name);
        return ShaderHandle{handle.idx};
    }

    auto RendererContext::create_shader(this RendererContext &self, const std::string &file_path) -> ShaderHandle
    {
        std::string data;
        std::string name;

        //TODO: File loading

        return self.create_shader(data, name);
    }
}