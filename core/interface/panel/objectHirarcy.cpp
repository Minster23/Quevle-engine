#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/renderer/renderer.hpp>
#include <utils/camera/camera.hpp>
#include <utils/font/IconsCodicons.h>
#include <core/interface/nodes.h>

using namespace QuavleEngine;
ObjectEntity entitiy;
Renderer rend;

void interface::objectHirarcy()
{
    ImGui::Begin("Object Hirarcy");

    // Right-click popup
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("Daalog");
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
    }

    if (ImGui::BeginPopup("Daalog"))
    {
        ImGui::Text("Choose");
        if (ImGui::Button("Load Light"))
        {
            rend.LoadAnotherLight();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    if (ImGui::CollapsingHeader("Utility"))
    {
        for (int g = 0; g < entitiy.lights.size(); g++)
        {
            ImGui::PushID(g);
            ImGui::TextUnformatted(ICON_CI_LIGHTBULB);
            ImGui::SameLine();

            std::string lightLabel = entitiy.lights[g].name + "##Light" + std::to_string(g);
            if (ImGui::Button(lightLabel.c_str()))
            {
                interface::utilityType = interface::UTILITY_TYPE::light;
                selectedNames.clear();
                InspectorIndexUtility = g;
                InspectorIndex = -1;
            }

            ImGui::SameLine();
            ImGui::Checkbox("##show", &entitiy.lights[g].isShow);
            ImGui::PopID();
        }

        for (int g = 1; g < cameras.size(); g++)
        {
            ImGui::PushID(1000 + g); // Offset ID to avoid collision
            ImGui::TextUnformatted(ICON_CI_DEVICE_CAMERA);
            ImGui::SameLine();

            std::string camLabel = cameras[g].name + "##Camera" + std::to_string(g);
            if (ImGui::Button(camLabel.c_str()))
            {
                interface::utilityType = interface::UTILITY_TYPE::camera;
                selectedNames.clear();
                InspectorIndexUtility = g;
                InspectorIndex = -1;
            }
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Objects"))
    {
        for (int k = 0; k < entitiy.objects.size(); k++)
        {
            bool isSelected = entitiy.objects[k].isSelected;
            ImGui::PushID(k);

            if (isSelected)
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

            ImGui::TextUnformatted(ICON_CI_SYMBOL_METHOD);
            ImGui::SameLine();

            std::string objLabel = entitiy.objects[k].name + "##Object" + std::to_string(k);
            if (ImGui::Button(objLabel.c_str()))
            {
                bool isCtrlDown = ImGui::GetIO().KeyCtrl;

                if (isCtrlDown)
                {
                    entitiy.objects[k].isSelected = !entitiy.objects[k].isSelected;
                    InspectorIndex = entitiy.objects[k].isSelected ? k : (InspectorIndex == k ? -1 : InspectorIndex);
                    selectedNames.clear();

                    for (const auto& obj : entitiy.objects)
                        if (obj.isSelected)
                            selectedNames.push_back(obj.name);
                }
                else
                {
                    for (auto& obj : entitiy.objects)
                        obj.isSelected = false;

                    entitiy.objects[k].isSelected = true;
                    InspectorIndex = k;
                    InspectorIndexUtility = -1;
                    selectedNames.clear();
                }
            }

            ImGui::SameLine();
            ImGui::Checkbox("## show", &entitiy.objects[k].isShow);

            if (isSelected)
                ImGui::PopStyleColor();

            ImGui::PopID();
        }
    }

    ImGui::End();
}
