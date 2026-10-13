cmake_minimum_required(VERSION 3.15)

project(LQengineProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(LQengine)
add_subdirectory(LQeditor)

add_subdirectory(external/spdlog)
add_subdirectory(external/unordered_dense)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL)

include("${CMAKE_SOURCE_DIR}/LQengine/cmake/ImGui.cmake")