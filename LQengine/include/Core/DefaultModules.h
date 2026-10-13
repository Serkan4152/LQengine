#pragma once
#include <tuple>
#include "Renderer/Renderer.h"
#include "UI/ImGui/ImGuiRenderer.h"

namespace LQ {
    using DefaultModules = std::tuple<Renderer, ImGuiRenderer>;  
}