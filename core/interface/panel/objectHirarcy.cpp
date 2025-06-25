#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/renderer/renderer.hpp>

using namespace QuavleEngine;
ObjectEntity entitiy;
Renderer rend;

void interface::objectHirarcy()
{
    ImGui::Begin("Object Hirarcy");
    if (ImGui::Button("Load Model"))
    {
        inputDebug("Info", "Loading model...");
        rend.loadModelFirst();
    }

    if (ImGui::Button("Load Light"))
    {
        inputDebug("Info", "Loading model...");
        rend.LoadAnotherLight();
    }

    if (ImGui::CollapsingHeader("Utility"))
    {
        for (int g = 0; g < entitiy.lights.size(); g++)
        {
            if (ImGui::Button((entitiy.lights[g].name + "##Light" + std::to_string(g)).c_str()))
            {
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
                InspectorIndex = k;
                InspectorIndexUtility = -1;
            }
        }
    }
    ImGui::End();
}