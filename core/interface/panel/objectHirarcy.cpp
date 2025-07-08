#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/renderer/renderer.hpp>
#include <utils/camera/camera.hpp>
#include <utils/font/IconsCodicons.h>

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
        if (ImGui::Button("add Light"))
        {
            rend.LoadAnotherLight();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("add Camera"))
        {
            addCamera();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("add Billboard"))
        {
            rend.loadBillboard();
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
                for (auto &obj : entitiy.objects)
                    obj.isSelected = false;

                for (auto &obj : cameras)
                    obj.isSelected = false;

                interface::utilityType = interface::UTILITY_TYPE::light;

                guizmoTarget = GUIZMOTARGET::LIGHT;
                entitiy.lights[g].isSelected = true;
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

                for (auto &obj : entitiy.objects)
                    obj.isSelected = false;

                for (auto &obj : entitiy.lights)
                    obj.isSelected = false;

                guizmoTarget = GUIZMOTARGET::CAMERA;
                cameras[g].isSelected = true;
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

                for (auto &obj : entitiy.lights)
                    obj.isSelected = false;
                
                
                for (auto &obj : cameras)
                    obj.isSelected = false;
                    
                InspectorIndexUtility = -1;

                if (isCtrlDown)
                {
                    entitiy.objects[k].isSelected = !entitiy.objects[k].isSelected;
                    InspectorIndex = entitiy.objects[k].isSelected ? k : (InspectorIndex == k ? -1 : InspectorIndex);
                    selectedNames.clear();

                    for (const auto &obj : entitiy.objects)
                        if (obj.isSelected)
                            selectedNames.push_back(obj.name);
                }
                else
                {
                    for (auto& obj : entitiy.objects)
                        obj.isSelected = false;

                    guizmoTarget = GUIZMOTARGET::OBJECT;
                    entitiy.objects[k].isSelected = true;
                    InspectorIndex = k;
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
