cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(BeLight)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)

include(FetchContent)

FetchContent_Declare(stb_image
  URL https://github.com/nothings/stb/archive/refs/heads/master.zip
  SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/stb-src
)

FetchContent_Declare(imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.tar.gz
  SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/imgui-src
)

FetchContent_MakeAvailable(stb_image imgui)

add_executable(
    BeLight
    src/main.cpp
    src/renderer.cpp
    src/mathematics.cpp
    src/simulation.cpp
    src/stb_impl.cpp
    src/text.cpp
    src/rasterizer.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

# Link the required system libraries to the executable
target_link_libraries(BeLight PRIVATE
    SDL2::SDL2
)

# Include the directories that contain headers for the linked libraries and fetched dependencies
target_include_directories(BeLight PRIVATE
    include
    ${CMAKE_BINARY_DIR}/_deps/stb-src
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
