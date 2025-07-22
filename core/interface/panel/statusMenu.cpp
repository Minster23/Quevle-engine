#include <core/interface/interface.hpp>

using namespace QuavleEngine;

void interface::statusMenu()
{
    ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("||");
    ImGui::SameLine();
    ImGui::Text("Condition ");
    ImGui::SameLine();
    if (play)
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Playing");
    ImGui::SameLine();
    ImGui::Text("||");

    ImGui::End();
}