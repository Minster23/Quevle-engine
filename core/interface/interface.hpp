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
 #define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <utils/camera/camera.hpp>

namespace QuavleEngine
{
    extern bool play;
    extern int cameraIndex;
    class WindowManager;

    class interface
    {
    public:
        void init(GLFWwindow *window, WindowManager *windowManager);
        void interfaceRender();
        void interfaceCleanup();
        void inputDebug(const std::string &typeOfDebug, const std::string &text);
        const ImGuiViewport* viewport;
        static float cameraSpeed;

        enum GUIZMOTARGET
        {
            BILLBOARD,
            OBJECT,
            LIGHT,
            CAMERA
        };

        GUIZMOTARGET guizmoTarget;

        //* -=========Guizmo Setting ===========
        void guizmoSetting(glm::mat4& modelMatrix, const glm::mat4& view, const glm::mat4& projection, ImVec2 pos, ImVec2 size, GUIZMOTARGET target);

        static bool isCodeEditor;

        void loadCode(std::string& path);

    private:
        WindowManager *m_windowManager = nullptr;
        ImVec2 m_viewportSize = {0.0f, 0.0f};
        void styleInterface();

        enum UTILITY_TYPE{
            billboard,
            camera,
            light
        };

        enum OBJECT_TYPE{
            single,
            multi
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
        int InspectorIndex = -1;
        int InspectorIndexUtility = -1;
        std::vector<std::string> selectedNames;

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

        //* -=========Text Editor===========
        void codeEditor();
        std::string codePath;

        //* -=========Scene Config===========
        void sceneConfig();

        //* -=========Status Menu===========
        void statusMenu();
    };
}