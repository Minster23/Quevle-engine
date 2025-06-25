#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <core/window/window.hpp>
#include <utils/camera/camera.hpp>


using namespace QuavleEngine;
Renderer render;
void interface::cameraConfig(){
    ImGui::Begin("Scene Config", nullptr, ImGuiWindowFlags_NoBackground);
    ImGui::Checkbox("Grid? ", &render.Grid);
    ImGui::End();
}