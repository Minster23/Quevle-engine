#include <core/interface/interface.hpp>
#include <utils/font/IconsCodicons.h>
#include <core/renderer/renderer.hpp>

using namespace QuavleEngine;
Renderer rendder;

void interface::sceneConfig()
{
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));

    float buttonWidth = 32.0f; // Approximate width (depends on font size)
    float availWidth = ImGui::GetContentRegionAvail().x;
    float offsetX = (availWidth - buttonWidth) * 0.5f;
    if (offsetX > 0)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

    if (!play)
    {
        if (ImGui::Button(ICON_CI_PLAY))
            play = true;
    }
    else
    {
        if (ImGui::Button(ICON_CI_STOP))
            play = false;
    }
    ImGui::SameLine();
    ImGui::Checkbox("Grid", &rendder.Grid);
    ImGui::SameLine();
    ImGui::ColorEdit3("Grid Color", glm::value_ptr(rendder.gridColor));
    ImGui::SameLine();
    ImGui::DragFloat("Grid Spacing", &rendder.gridSpacing, 0.1f, 0.1f, 10.0f);

    if (play) {
        // Find the index of the MASTER camera when playing
        for (size_t i = 0; i < cameras.size(); ++i) {
            if (cameras[i].type == CAMERATYPE::MASTER && cameras[i].type != CAMERATYPE::NONE) {
                cameraIndex = i;
                break;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < cameras.size(); ++i)
        {
            if (cameras[i].type == CAMERATYPE::ENGINE && cameras[i].type != CAMERATYPE::NONE && cameras[i].type != CAMERATYPE::MASTER)
            {
                cameraIndex = i;
                break;
            }
        }
        play = false;
    }

    ImGui::PopStyleVar();
    ImGui::End();
}
