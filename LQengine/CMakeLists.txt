cmake_minimum_required(VERSION 3.15)

project(LQengine VERSION 0.1.0 LANGUAGES CXX)

file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/ImGui/*.cpp"
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES}
)

###
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        spdlog::spdlog
        unordered_dense::unordered_dense
        SDL3::SDL3
        bgfx
        bimg
        bx
        imgui
)

target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        /utf-8
        /Zc:__cplusplus
        /Zc:preprocessor
    >
    
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall
        -Wextra
    >
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    ENGINE_VERSION="${PROJECT_VERSION}"
    ENGINE_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
    ENGINE_VERSION_MINOR="${PROJECT_VERSION_MINOR}"
    ENGINE_VERSION_PATCH="${PROJECT_VERSION_PATCH}"

    $<$<CONFIG:Debug>:BX_CONFIG_DEBUG=0>
    $<$<CONFIG:Release>:BX_CONFIG_DEBUG=0>
)
