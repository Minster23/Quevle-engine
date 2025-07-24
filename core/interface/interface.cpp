#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <utils/Debug.h>
#include <utils/font/IconsCodicons.h>
#include <core/window/window.hpp>
#include <utils/camera/camera.hpp>
#include <GLFW/glfw3.h>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/saveData/savedData.hpp>
#include <core/scene/scene.hpp>

namespace QuavleEngine{
    bool play = false;
    int cameraIndex = 0;
};

using namespace SCENEMANAGER;
using namespace QuavleEngine;

Renderer rendeeers;
const GLFWvidmode *mode;
ObjectEntity objectsEntity;
bool interface::isCodeEditor = false;

saveData save;

void interface::init(GLFWwindow *window, WindowManager* windowManager)
{
    m_windowManager = windowManager;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
    ImPlot::CreateContext();
    styleInterface();
    

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    DEBUG_PRINT("UI INIT");
}

void interface::styleInterface() {
    ImGuiIO& io = ImGui::GetIO();

    // 1. Load main font: Roboto Condensed
    ImFontConfig roboto_config;
    roboto_config.MergeMode = false;
    roboto_config.PixelSnapH = true;
    roboto_config.OversampleH = roboto_config.OversampleV = 3;
    roboto_config.RasterizerMultiply = 1.1f;

    ImFont* mainFont = io.Fonts->AddFontFromFileTTF(
        "E:/QuavleEngine/utils/font/Roboto_Condensed-Medium.ttf",
        16.0f * io.DisplayFramebufferScale.x, &roboto_config);

    // 2. Merge Codicon into Roboto
    static const ImWchar codicon_ranges[] = { ICON_MIN_CI, ICON_MAX_CI, 0 };

    ImFontConfig icon_config;
    icon_config.MergeMode = true;
    icon_config.PixelSnapH = true;
    icon_config.GlyphMinAdvanceX = 14.0f * io.DisplayFramebufferScale.x; // icon spacing

    io.Fonts->AddFontFromFileTTF(
        "E:/QuavleEngine/utils/font/" FONT_ICON_FILE_NAME_CI,
        15.0f * io.DisplayFramebufferScale.x,
        &icon_config, codicon_ranges);

    // 3. Apply Nier Automata inspired style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 16.0f;
    style.GrabMinSize = 8.0f;
    style.WindowRounding = 8.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.PopupRounding = 6.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.10f, 0.10f, 0.12f, 0.95f);
    colors[ImGuiCol_Border]                = ImVec4(0.20f, 0.20f, 0.23f, 0.35f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.20f, 0.22f, 0.25f, 0.78f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.06f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.18f, 0.19f, 0.21f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 0.26f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.28f, 0.29f, 0.32f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.27f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered]            = ImVec4(0.18f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_TabActive]             = ImVec4(0.16f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_TabUnfocused]          = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);
}

void interface::interfaceRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu(ICON_CI_FILE " File")) {
            if (ImGui::MenuItem(ICON_CI_SAVE "Save", "Ctrl+S")) { save.save(); }
            if (ImGui::MenuItem(ICON_CI_CLOSE " Exit", "Alt+F4")) { save.save(); exit(0); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_CI_EDIT " Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();


    consolePanel();
    objectHirarcy();
    inspector();
    profiler();
    fileExplorer();
    sceneConfig();
    statusMenu();

    {
        ImGui::Begin("Scene Manager", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Scenes");
        ImGui::Separator();
        for(int i = 0; i < SCENEMANAGER::scene::scenes.size(); i++){
            if(SCENEMANAGER::scene::scenes[i].isLoaded){
                ImGui::Text("%s", SCENEMANAGER::scene::scenes[i].name.c_str());

                ImGui::Text("UUID: %c", SCENEMANAGER::scene::scenes[i].UUID);

                ImGui::Text("Objects: %zu", SCENEMANAGER::scene::scenes[i].objectLoctions.size());
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Object locations in this scene");
                    for (const auto &obj : SCENEMANAGER::scene::scenes[i].objectLoctions)
                    {
                        ImGui::BulletText("%s", obj.c_str());
                    }
                    ImGui::EndTooltip();
                }

                ImGui::Text("Lights: %zu", SCENEMANAGER::scene::scenes[i].lights.size());
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Lights in this scene");
                    for (const auto &light : SCENEMANAGER::scene::scenes[i].lights)
                    {
                        ImGui::BulletText("Light ID: %d", light.name);
                    }
                    ImGui::EndTooltip();
                }

                ImGui::Text("Cubemaps: %zu", SCENEMANAGER::scene::scenes[i].cubeMaps.size());
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Cubemaps in this scene");
                    ImGui::EndTooltip();
                }

                ImGui::Text("Billboards: %zu", SCENEMANAGER::scene::scenes[i].billboards.size());
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Billboards in this scene");
                    for (const auto &billboard : SCENEMANAGER::scene::scenes[i].billboards)
                    {
                        ImGui::BulletText("Billboard ID: %d", billboard.name); 
                    }
                    ImGui::EndTooltip();
                }

                ImGui::Text("Cameras: %zu", SCENEMANAGER::scene::scenes[i].cameras.size());
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Cameras in this scene");
                    for (const auto &camera : SCENEMANAGER::scene::scenes[i].cameras)
                    {
                        ImGui::BulletText("Camera ID: %d", camera.name); 
                    }
                    ImGui::EndTooltip();
                }
            }
        }
        ImGui::End();
    }


    if (isCodeEditor)
        codeEditor();

    // Call gizmo


    ImGui::ShowDemoWindow();

    ImGui::Begin("My Scene", nullptr, ImGuiWindowFlags_NoTitleBar);

    if(ImGui::IsWindowHovered()){
        WindowManager::isInteractive = true;
    }
    else{
        WindowManager::isInteractive = false;
    }


    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_viewportSize.x != viewportPanelSize.x || m_viewportSize.y != viewportPanelSize.y)
    {
        if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
        {
            m_viewportSize = viewportPanelSize;
            m_windowManager->rescale_framebuffer(m_viewportSize.x, m_viewportSize.y);
        }
    }

    glViewport(0, 0, (int)m_viewportSize.x, (int)m_viewportSize.y);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void *)(intptr_t)m_windowManager->get_texture_id(),
        pos,
        ImVec2(pos.x + m_viewportSize.x, pos.y + m_viewportSize.y),
        ImVec2(0, 1),
        ImVec2(1, 0));

    if (!play)
    {
        if (InspectorIndex >= 0 && InspectorIndex < objectsEntity.objects.size())
        {
            if (objectsEntity.objects[InspectorIndex].isSelected && guizmoTarget == GUIZMOTARGET::OBJECT)
            {
                guizmoSetting(rendeeers.model, cameras[0].view, rendeeers.projection, pos, m_viewportSize, guizmoTarget);
            }
        }

        if (InspectorIndexUtility >= 0 && InspectorIndexUtility < objectsEntity.lights.size())
        {
            if (objectsEntity.lights[InspectorIndexUtility].isSelected && guizmoTarget == GUIZMOTARGET::LIGHT)
            {
                guizmoSetting(rendeeers.modelLight, cameras[0].view, rendeeers.projection, pos, m_viewportSize, guizmoTarget);
            }
        }
        if (InspectorIndexUtility >= 0 && InspectorIndexUtility < cameras.size())
        {
            if (cameras[InspectorIndexUtility].isSelected && guizmoTarget == GUIZMOTARGET::CAMERA)
            {
                guizmoSetting(cameras[InspectorIndexUtility].model, cameras[0].view, rendeeers.projection, pos, m_viewportSize, guizmoTarget);
            }
        }
        if (InspectorIndexUtility >= 0 && InspectorIndexUtility < objectsEntity.billboards.size())
        {
            if (objectsEntity.billboards[InspectorIndexUtility].isSelected && guizmoTarget == GUIZMOTARGET::BILLBOARD)
            {
                guizmoSetting(objectsEntity.billboards[InspectorIndexUtility].model, cameras[0].view, rendeeers.projection, pos, m_viewportSize, guizmoTarget);
            }
        }
    }

    ImGui::End();

    // Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void interface::interfaceCleanup()
{
    DEBUG_PRINT("UI DESTROYED");
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}