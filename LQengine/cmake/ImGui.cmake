set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")
set(IMGUI_BACKEND_DIR "${CMAKE_CURRENT_LIST_DIR}/../src/UI/ImGui")

add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui.h"
    "${IMGUI_DIR}/imgui_internal.h"
    "${IMGUI_DIR}/imconfig.h"
    "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdl3.h"
    "${IMGUI_BACKEND_DIR}/imgui_impl_bgfx.cpp"
    "${IMGUI_BACKEND_DIR}/imgui_impl_bgfx.h"
    "${IMGUI_BACKEND_DIR}/fs_ocornut_imgui.bin.h"
    "${IMGUI_BACKEND_DIR}/vs_ocornut_imgui.bin.h"
)

target_include_directories(imgui
    PUBLIC
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
        "${IMGUI_BACKEND_DIR}"
)

target_link_libraries(imgui
    PRIVATE
        SDL3::SDL3
        bgfx
        bimg
        bx
)
