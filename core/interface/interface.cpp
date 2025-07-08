#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <utils/Debug.h>
#include <utils/font/IconsCodicons.h>
#include <core/window/window.hpp>
#include <utils/camera/camera.hpp>
#include <GLFW/glfw3.h>
#include <core/renderer/entity/objectEntity.hpp>

namespace QuavleEngine{
    bool play = false;
    int cameraIndex = 0;
};


using namespace QuavleEngine;
Renderer rendeeers;
const GLFWvidmode *mode;
ObjectEntity objectsEntity;
bool interface::isCodeEditor = false;



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

    io.Fonts->AddFontDefault(); // Load default ImGui font as the base
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    static const ImWchar icons_ranges[] = { ICON_MIN_CI, ICON_MAX_CI, 0 };
    io.Fonts->AddFontFromFileTTF("D:/QuavleEngine/utils/font/" FONT_ICON_FILE_NAME_CI, 15.0f * io.DisplayFramebufferScale.x, &icons_config, icons_ranges);
    

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    DEBUG_PRINT("UI INIT");
}

void interface::styleInterface()
{
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    // Base Colors
    ImVec4 bgColor = ImVec4(0.10f, 0.105f, 0.11f, 1.00f);
    ImVec4 lightBgColor = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    ImVec4 panelColor = ImVec4(0.17f, 0.18f, 0.19f, 1.00f);
    ImVec4 panelHoverColor = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    ImVec4 panelActiveColor = ImVec4(0.23f, 0.26f, 0.29f, 1.00f);
    ImVec4 textColor = ImVec4(0.86f, 0.87f, 0.88f, 1.00f);
    ImVec4 textDisabledColor = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    ImVec4 borderColor = ImVec4(0.14f, 0.16f, 0.18f, 1.00f);

    // Text
    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_TextDisabled] = textDisabledColor;

    // Windows
    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_PopupBg] = bgColor;
    colors[ImGuiCol_Border] = borderColor;
    colors[ImGuiCol_BorderShadow] = borderColor;

    // Headers
    colors[ImGuiCol_Header] = panelColor;
    colors[ImGuiCol_HeaderHovered] = panelHoverColor;
    colors[ImGuiCol_HeaderActive] = panelActiveColor;

    // Buttons
    colors[ImGuiCol_Button] = panelColor;
    colors[ImGuiCol_ButtonHovered] = panelHoverColor;
    colors[ImGuiCol_ButtonActive] = panelActiveColor;

    // Frame BG
    colors[ImGuiCol_FrameBg] = lightBgColor;
    colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
    colors[ImGuiCol_FrameBgActive] = panelActiveColor;

    // Tabs
    colors[ImGuiCol_Tab] = panelColor;
    colors[ImGuiCol_TabHovered] = panelHoverColor;
    colors[ImGuiCol_TabActive] = panelActiveColor;
    colors[ImGuiCol_TabUnfocused] = panelColor;
    colors[ImGuiCol_TabUnfocusedActive] = panelHoverColor;

    // Title
    colors[ImGuiCol_TitleBg] = bgColor;
    colors[ImGuiCol_TitleBgActive] = bgColor;
    colors[ImGuiCol_TitleBgCollapsed] = bgColor;

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = bgColor;
    colors[ImGuiCol_ScrollbarGrab] = panelColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = panelHoverColor;
    colors[ImGuiCol_ScrollbarGrabActive] = panelActiveColor;

    // Checkmark
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    // Slider
    colors[ImGuiCol_SliderGrab] = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive] = panelActiveColor;

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = panelColor;
    colors[ImGuiCol_ResizeGripHovered] = panelHoverColor;
    colors[ImGuiCol_ResizeGripActive] = panelActiveColor;

    // Separator
    colors[ImGuiCol_Separator] = borderColor;
    colors[ImGuiCol_SeparatorHovered] = panelHoverColor;
    colors[ImGuiCol_SeparatorActive] = panelActiveColor;

    // Plot
    colors[ImGuiCol_PlotLines] = textColor;
    colors[ImGuiCol_PlotLinesHovered] = panelActiveColor;
    colors[ImGuiCol_PlotHistogram] = textColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelActiveColor;

    // Text Selected BG
    colors[ImGuiCol_TextSelectedBg] = panelActiveColor;

    // Modal Window Dim Bg
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.105f, 0.11f, 0.5f);

    // Tables
    colors[ImGuiCol_TableHeaderBg] = panelColor;
    colors[ImGuiCol_TableBorderStrong] = borderColor;
    colors[ImGuiCol_TableBorderLight] = borderColor;
    colors[ImGuiCol_TableRowBg] = bgColor;
    colors[ImGuiCol_TableRowBgAlt] = lightBgColor;

    // Styles
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 2.0f;
    style.WindowBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabMinSize = 7.0f;
    style.GrabRounding = 2.0f;
    style.TabBorderSize = 1.0f;
    style.TabRounding = 2.0f;

    // Reduced Padding and Spacing
    style.WindowPadding = ImVec2(5.0f, 5.0f);
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
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
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
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
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* handle open */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* handle save */ }
            if (ImGui::MenuItem("Exit", "Alt+F4")) { /* handle exit */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
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

    if (isCodeEditor)
        codeEditor();

    // Call gizmo


    ImGui::ShowDemoWindow();

    ImGui::Begin("My Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

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