#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>

using namespace QuavleEngine;

ObjectEntity entity;

void interface::inspector()
{
    ImGui::Begin("Inspector");
    if (InspectorIndex > 0 && InspectorIndexUtility == -1)
    {
        ImGui::Checkbox("Show", &entity.objects[InspectorIndex].isShow);
        ImGui::Separator();
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::Text(entity.objects[InspectorIndex].name.c_str());
        ImGui::Separator();

        // Position
        ImGui::Text("Position:");
        ImGui::DragFloat("X", &entity.objects[InspectorIndex].position.x);
        ImGui::DragFloat("Y ", &entity.objects[InspectorIndex].position.y); // Added space to differentiate label
        ImGui::DragFloat("Z  ", &entity.objects[InspectorIndex].position.z); // Added spaces to differentiate label
        ImGui::Separator();
        // Scale
        ImGui::Text("Scale");
        ImGui::DragFloat("X ", &entity.objects[InspectorIndex].scale.x); // Added space to differentiate label
        ImGui::DragFloat("Y  ", &entity.objects[InspectorIndex].scale.y); // Added spaces to differentiate label
        ImGui::DragFloat("Z   ", &entity.objects[InspectorIndex].scale.z); // Added spaces to differentiate label
        ImGui::Separator();
        
        // Rotation
        ImGui::Text("Rotation");
        ImGui::DragFloat("X    ", &entity.objects[InspectorIndex].rotation.x); // Added spaces to differentiate label
        ImGui::DragFloat("Y     ", &entity.objects[InspectorIndex].rotation.y); // Added spaces to differentiate label
        ImGui::DragFloat("Z      ", &entity.objects[InspectorIndex].rotation.z); // Added spaces to differentiate label
        ImGui::Separator();
        
        ImGui::Text("Texture");

        ImGui::Text("Diffuse");
        ImGui::SameLine();
        ImGui::Image((void *)(intptr_t)entity.objects[InspectorIndex].diffuseTextureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
        ImGui::Separator();

        ImGui::Text("Specular");
        ImGui::SameLine();
        ImGui::Image((void *)(intptr_t)entity.objects[InspectorIndex].specularTextureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
        ImGui::Separator();

        ImGui::Text("Normal");
        ImGui::SameLine();
        ImGui::Image((void *)(intptr_t)entity.objects[InspectorIndex].normalTextureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
        ImGui::Separator();

        ImGui::Text("Metallic");
        ImGui::SameLine();
        ImGui::Image((void *)(intptr_t)entity.objects[InspectorIndex].metallicTextureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
        ImGui::Separator();

        ImGui::Text("Roughness");
        ImGui::SameLine();
        ImGui::Image((void *)(intptr_t)entity.objects[InspectorIndex].roughnessTextureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
        ImGui::Separator();

    }
    if (InspectorIndexUtility > -1 && InspectorIndex == -1){
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::Text(entity.lights[InspectorIndexUtility].name.c_str());
        ImGui::Separator();
        ImGui::Text("Position:");
        ImGui::DragFloat("X", &entity.lights[InspectorIndexUtility].position.x);
        ImGui::DragFloat("Y ", &entity.lights[InspectorIndexUtility].position.y); 
        ImGui::DragFloat("Z  ", &entity.lights[InspectorIndexUtility].position.z);
        ImGui::Separator();
        ImGui::Text("Intesisty: ");
        ImGui::DragFloat("Intensity  ", &entity.lights[InspectorIndexUtility].intensity);
        ImGui::Separator();
        ImGui::ColorEdit3("Color", &entity.lights[InspectorIndexUtility].lightColor.x);
    }
    ImGui::End();
}