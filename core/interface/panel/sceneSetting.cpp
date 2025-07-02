#include <core/interface/interface.hpp>
#include <utils/font/IconsCodicons.h>

using namespace QuavleEngine;

void interface::sceneConfig() {
    ImGui::Begin("Scene Config", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));

    if (!play) { // Use the 'play' variable
        if (ImGui::Button(ICON_CI_PLAY)) { // Use the 'play' variable
            play = true; // Use the 'play' variable
        }
        ImGui::SameLine(); // Use the 'play' variable
        ImGui::Text("Stopped");
    } else {
        if (ImGui::Button(ICON_CI_STOP)) {
            play = false;
        }
        ImGui::SameLine();
        ImGui::Text("Running...");
    }

    ImGui::PopStyleVar();
    ImGui::End();
}