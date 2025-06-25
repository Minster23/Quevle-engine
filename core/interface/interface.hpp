#pragma once

#include <core/renderer/shader_h.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <iostream>
#include <vector>
#include <string>

namespace QuavleEngine
{
    class WindowManager;

    class interface
    {
    public:
        void init(GLFWwindow *window, WindowManager* windowManager);
        void interfaceRender();
        void interfaceCleanup();
        void inputDebug(const std::string& typeOfDebug, const std::string& text);

    private:
        WindowManager* m_windowManager = nullptr;
        ImVec2 m_viewportSize = { 0.0f, 0.0f };

        //* -=========DEBUG===========
        struct messageDebug
        {
            std::string text;
            std::string type;
        };
        std::vector<messageDebug> message;
        void consolePanel();

        //* -=========Object Hirarcy===========
        void objectHirarcy();


        //* -=========Inspector===========
        void inspector();
        int InspectorIndex;
        int InspectorIndexUtility;

        //* -=========Camera Config===========
        void cameraConfig();
    };
}