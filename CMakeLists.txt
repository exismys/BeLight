cmake_minimum_required(VERSION 3.20)

Project(BeLight)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)

add_executable(
    BeLight
    src/main.cpp
    src/renderer.cpp
    src/mathematics.cpp
)

# Link the required system libraries to the executable
target_link_libraries(BeLight PRIVATE
    SDL2::SDL2
)

# Include the directories that contain headers for the linked libraries and fetched dependencies
target_include_directories(BeLight PRIVATE
    include
)