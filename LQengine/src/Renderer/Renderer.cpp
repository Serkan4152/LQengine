#include "Renderer/Renderer.h"
#include "Core/App.h"
#include "Window/Window.h"

#include "bgfx/bgfx.h"

namespace LQ {
    auto Renderer::init(this Renderer& self) -> std::expected<void, std::string> {
        self.m_context = &App::get()->get_rendercontext();
        return {};  
    }

    auto Renderer::deinit(this Renderer& self) -> std::expected<void, std::string> {
        return {};
    }

    auto Renderer::update(this Renderer& self) -> void {
        
    }
}