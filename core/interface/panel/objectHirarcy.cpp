#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/renderer/renderer.hpp>
#include <utils/camera/camera.hpp>

using namespace QuavleEngine;
ObjectEntity entitiy;
Renderer rend;

void interface::objectHirarcy()
{
    ImGui::Begin("Object Hirarcy");
    // if (ImGui::Button("Load Model"))
    // {
    //     rend.loadModelFirst();
    // }
    ImGui::SameLine();
    if (ImGui::Button("Load Light"))
    {
        rend.LoadAnotherLight();
    }

    if (ImGui::CollapsingHeader("Utility"))
    {
        for (int g = 0; g < entitiy.lights.size(); g++)
        {
            if (ImGui::Button((entitiy.lights[g].name + "##Light" + std::to_string(g)).c_str()))
            {
                interface::utilityType = interface::UTILITY_TYPE::light;
                InspectorIndexUtility = g;
                InspectorIndex = -1;
            }
        }

        for (int g = 1; g < cameras.size(); g++)
        {
            if (ImGui::Button((cameras[g].name + "##Camera" + std::to_string(g)).c_str()))
            {
                interface::utilityType = interface::UTILITY_TYPE::camera;
                InspectorIndexUtility = g;
                InspectorIndex = -1;
            }
        }
    }

    if (ImGui::CollapsingHeader("Objects"))
    {
        for (int k = 0; k < entitiy.objects.size(); k++)
        {
            if (ImGui::Button((entitiy.objects[k].name + "##Object" + std::to_string(k)).c_str()))
            {
                if (InspectorIndex == k)
                {
                    // If already selected, unselect it
                    entitiy.objects[k].isSelected = false;
                    InspectorIndex = -1;
                }
                else
                {
                    // Unselect all
                    for (int i = 0; i < entitiy.objects.size(); ++i)
                        entitiy.objects[i].isSelected = false;

                    // Select this one
                    entitiy.objects[k].isSelected = true;
                    InspectorIndex = k;
                    InspectorIndexUtility = -1;
                }
            }
        }
    }

    ImGui::End();
}