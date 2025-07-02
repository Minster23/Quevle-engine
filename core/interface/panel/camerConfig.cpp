#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <core/window/window.hpp>
#include <utils/camera/camera.hpp>


using namespace QuavleEngine;
Renderer render;
void interface::cameraConfig(){
    ImGui::Begin("Scene Config", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::Checkbox("Grid? ", &render.Grid);
    ImGui::SameLine();
    ImGui::ColorEdit3("Grid Color", glm::value_ptr(render.gridColor));
    ImGui::DragFloat("Grid Spacing", &render.gridSpacing, 0.1f, 0.1f, 10.0f);
    ImGui::End();
}