#pragma once

#include <core/renderer/shader_h.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <utils/font/IconsCodicons.h>
#include <filesystem>
#include <implot.h>
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace QuavleEngine
{
    extern bool play;
    class WindowManager;

    class interface
    {
    public:
        void init(GLFWwindow *window, WindowManager *windowManager);
        void interfaceRender();
        void interfaceCleanup();
        void inputDebug(const std::string &typeOfDebug, const std::string &text);
        const ImGuiViewport *viewport;

    private:
        WindowManager *m_windowManager = nullptr;
        ImVec2 m_viewportSize = {0.0f, 0.0f};
        void styleInterface();

        enum UTILITY_TYPE{
            camera,
            light
        };

        UTILITY_TYPE utilityType;

        //* -=========DEBUG===========
        struct messageDebug
        {
            std::string text;
            std::string type;
        };
        static std::vector<messageDebug> message;
        void consolePanel();
        void updateConsole();

        //* -=========Object Hirarcy===========
        void objectHirarcy();

        //* -=========Inspector===========
        void inspector();
        int InspectorIndex;
        int InspectorIndexUtility;

        //* -=========Camera Config===========
        void cameraConfig();

        //* -=========Profiler===========
        void profiler();

        //* -=========File Explorer===========
        void fileExplorer();
        std::string currentDirectory = std::filesystem::current_path().string();
        const char *iconTrash = ICON_CI_TRASHCAN;
        const char *iconShow = ICON_CI_EYE_WATCH;
        const char *iconPosition = ICON_CI_MOVE;

        //* -=========File Explorer===========
        void nodePanel();

        //* -=========Text Editor===========
        void codeEditor();
        void loadCode(std::string path);
        std::string codePath;

        //* -=========Scene Config===========
        void sceneConfig();
    };
}