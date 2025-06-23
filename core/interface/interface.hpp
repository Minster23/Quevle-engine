#pragma once

#include <core/renderer/shader_h.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace QuavleEngine
{
    // Forward declaration to avoid circular dependency
    class WindowManager;

    class interface
    {
    public:
        void init(GLFWwindow *window, WindowManager* windowManager);
        void interfaceRender();
        void interfaceCleanup();

    private:
        WindowManager* m_windowManager = nullptr;
        ImVec2 m_viewportSize = { 0.0f, 0.0f };
    };
}