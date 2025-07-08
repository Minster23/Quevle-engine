#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <core/window/window.hpp>
#include <utils/camera/camera.hpp>


using namespace QuavleEngine;
float interface::cameraSpeed = 1.0f;
Renderer render;
void interface::cameraConfig(){
    ImGui::Begin("Scene Config", nullptr);
    ImGui::Checkbox("Grid? ", &render.Grid);
    ImGui::SameLine();
    ImGui::ColorEdit3("Grid Color", glm::value_ptr(render.gridColor));
    ImGui::DragFloat("Grid Spacing", &render.gridSpacing, 0.1f, 0.1f, 10.0f);
    ImGui::DragFloat("Camera Speed", &interface::cameraSpeed, 0.1f, 0.1f, 100.0f);
    ImGui::End();
}