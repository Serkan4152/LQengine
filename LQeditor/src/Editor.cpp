#include "Editor.h"
#include "Core/App.h"
#include "Utils/Log.h"

#include "imgui.h"

namespace LQ {
    auto Editor::init(this Editor& self) -> std::expected<void, std::string> {
        return {};
    }

    auto Editor::deinit(this Editor& self) -> std::expected<void, std::string> {
        return {};
    }

    auto Editor::update(this Editor& self) -> void {
        auto& renderer_context = App::get_rendercontext();

        renderer_context.new_frame();

        ImGui::ShowDemoWindow();  
        
        renderer_context.end_frame();
    }
}