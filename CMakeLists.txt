cmake_minimum_required(VERSION 3.10)
project(QuavleEngine VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main application source
set(MAIN_SOURCES
    src/main.cpp
)

# include FETCHCONTENT
include(FetchContent)

# Find threading support
find_package(Threads REQUIRED)

# Fetch pentru JSON
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

# ImGui sources
file(GLOB IMGUI_SOURCES
    vendor/imgui/*.cpp
    vendor/imgui/*.h
    vendor/imgui/backends/imgui_impl_glfw.h
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.h
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB ASSIMP_SOURCES

)

# Engine sources (recursive in subfolders)
file(GLOB_RECURSE ENGINE_SOURCES
    core/*.cpp core/*.hpp
    application/*.cpp application/*.hpp
    utils/camera/*.cpp utils/camera/*.hpp
    utils/lightning/*.cpp utils/lightning/*.hpp
    utils/*.cpp utils/*.hpp utils/*.h
    core/renderer/*.cpp core/renderer/*.hpp
    core/model/*.cpp core/model/*.hpp
)

file(GLOB_RECURSE IMPLOT
    vendor/implot-master/*.h
    vendor/implot-master/*.cpp
)

file(GLOB_RECURSE EDITOR
    vendor/editor/*.cpp
    vendor/editor/*.h
)

file(GLOB_RECURSE GLAD_SOURCES
    vendor/glad/src/*.c
    vendor/glad/include/glad/*.h
    vendor/glad/include/KHR/*.h
)

file(GLOB_RECURSE GUIZMO_SOURCES
    vendor/ImGuizmo/*.cpp
    vendor/ImGuizmo/*.h
)

# Create executable
add_executable(QuavleEngine
    ${MAIN_SOURCES}
    ${IMGUI_SOURCES}
    ${ENGINE_SOURCES}
    ${GLAD_SOURCES}
    ${IMPLOT}
    ${EDITOR}
    ${GUIZMO_SOURCES}
    # ${VK_BOOTSTRAP_SOURCES}
)

# Compiler options
target_compile_options(QuavleEngine PRIVATE -fpermissive)

# Enable threading support and async features
target_compile_definitions(QuavleEngine PRIVATE
    -DTHREAD_SUPPORT_ENABLED
    -DASYNC_SUPPORT_ENABLED
)

# For MinGW/GCC on Windows, enable pthread support
if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(QuavleEngine PRIVATE -pthread)
    target_link_options(QuavleEngine PRIVATE -pthread)
endif()

# Lua
set(LUA_INCLUDE_DIR "F:/Devtool/mysys2/ucrt64/include")
set(LUA_LIBRARY "F:/Devtool/mysys2/ucrt64/lib/liblua.dll.a")

# Include directories
target_include_directories(QuavleEngine PRIVATE
    ${CMAKE_SOURCE_DIR}                                # Headers in root
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.WIN64/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/vk-bootstrap/src
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor/stb
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/implot-master
    ${CMAKE_SOURCE_DIR}/vendor/editor
    ${CMAKE_SOURCE_DIR}/vendor/ImGuizmo
    ${LUA_INCLUDE_DIR}
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

# Link all libraries including threading support
target_link_libraries(QuavleEngine
    ${LUA_LIBRARY}
    glfw
    assimp
    ${OPENGL_gl_LIBRARY}
    comctl32
    nlohmann_json::nlohmann_json
    Threads::Threads
)

# Platform-specific threading libraries
if(WIN32)
    # Windows-specific threading support
    if(CMAKE_COMPILER_IS_GNUCXX)
        # MinGW requires explicit pthread linking
        target_link_libraries(QuavleEngine pthread)
    endif()
elseif(UNIX)
    # Linux/Unix pthread support
    target_link_libraries(QuavleEngine pthread)
endif()

# Optional: show sources in IDE with folders
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${MAIN_SOURCES} ${IMGUI_SOURCES} ${ENGINE_SOURCES})