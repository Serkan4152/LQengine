#include "UI/ImGui/ImGuiRenderer.h"
#include "Core/App.h"
#include "Window/Window.h"

#include "imgui_impl_sdl3.h"
#include "imgui_impl_bgfx.h"

namespace LQ {
    auto ImGuiRenderer::init(this ImGuiRenderer &self) -> std::expected<void, std::string>
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

        ImGui_Implbgfx_Init(0);

        auto window = (SDL_Window *)App::get()->get_window().get_handle();
        if (!ImGui_ImplSDL3_InitForVulkan(window)) {
            return std::unexpected("Failed to initialize ImGui SDL3 implementation.");
        }
        
        return {};
    }

    auto ImGuiRenderer::deinit(this ImGuiRenderer &self) -> std::expected<void, std::string>
    {
        ImGui_ImplSDL3_Shutdown();
        ImGui_Implbgfx_Shutdown();

        ImGui::DestroyContext();
        return {};
    }

    auto ImGuiRenderer::begin_frame(this ImGuiRenderer &self) -> void
    {
        ImGui_Implbgfx_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();
    }

    auto ImGuiRenderer::end_frame(this ImGuiRenderer &self) -> void
    {
        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
    }

    auto ImGuiRenderer::process_event(this ImGuiRenderer &self, const void *event) -> void
    {
        ImGui_ImplSDL3_ProcessEvent((SDL_Event*)event);
    }
}