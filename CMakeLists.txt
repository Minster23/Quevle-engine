cmake_minimum_required(VERSION 3.10)
project(QuavleEngine VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main application source
set(MAIN_SOURCES
    src/main.cpp
)

# Add vulkan sdk
set(VULKAN_SDK $ENV{VULKAN_SDK})


# ImGui sources
file(GLOB IMGUI_SOURCES
    vendor/imgui/*.cpp
)

# Engine sources (recursive in subfolders)
file(GLOB_RECURSE ENGINE_SOURCES
    core/*.cpp core/*.hpp
    application/*.cpp application/*.hpp
)

# file(GLOB_RECURSE VK_BOOTSTRAP_SOURCES
# vendor/vk-bootstrap/src/*.h
# vendor/vk-bootstrap/src/*.cpp
# )

file(GLOB_RECURSE GLAD_SOURCES
vendor/glad/*.c

)

# Create executable
add_executable(QuavleEngine
    ${MAIN_SOURCES}
    ${IMGUI_SOURCES}
    ${ENGINE_SOURCES}
    ${GLAD_SOURCES}
)

# Include directories
target_include_directories(QuavleEngine PRIVATE
    ${CMAKE_SOURCE_DIR}                                # Headers in root
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.WIN64/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/vk-bootstrap/src
    ${CMAKE_SOURCE_DIR}/vendor/glad
    ${CMAKE_SOURCE_DIR}/vendor/stb
)

# GLFW static library (MinGW assumed)
add_library(glfw STATIC IMPORTED)

set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.WIN64/lib-mingw-w64") # change if MSVC
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.WIN64/include")

set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_LIB_DIR}/libglfw3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
)

# OpenGL
find_package(OpenGL REQUIRED)


# # Vulkan libraries
# target_include_directories(QuavleEngine PRIVATE ${VULKAN_SDK}/Include)
# target_link_directories(QuavleEngine PRIVATE ${VULKAN_SDK}/Lib)

# add_subdirectory(vendor/vk-bootstrap)


# Link all libraries
target_link_libraries(QuavleEngine
    glfw
    ${OPENGL_gl_LIBRARY}
    # vk-bootstrap::vk-bootstrap
)

# Optional: show sources in IDE with folders
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${MAIN_SOURCES} ${IMGUI_SOURCES} ${ENGINE_SOURCES})
