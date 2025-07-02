#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/model.hpp>
#include <utils/camera/camera.hpp>


using namespace QuavleEngine;

ObjectEntity entity;

void interface::inspector()
{
    ImGui::Begin("Inspector");
    if (InspectorIndex > -1 && InspectorIndexUtility == -1)
    {
        std::string showLabel = std::string(std::string(iconShow) + " Show" );
        ImGui::Checkbox(showLabel.c_str(), &entity.objects[InspectorIndex].isShow);

        ImGui::SameLine();

        std::string delateLabel = std::string(std::string(iconTrash) + " Delete");
        if (ImGui::Button(delateLabel.c_str()))
        {
            InspectorIndex = -1; // Deselect the object after deletion
            entity.objects.erase(entity.objects.begin() + InspectorIndex);
        }
        ImGui::Separator();

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::Text(entity.objects[InspectorIndex].name.c_str());
        ImGui::Separator();

        if(ImGui::Button("copy UUID")){
            ImGui::SetClipboardText(entity.objects[InspectorIndex].UUID.c_str());
        }
        ImGui::SameLine();
        ImGui::Text(entity.objects[InspectorIndex].UUID.c_str());

        ImGui::Separator();

        // Position
        std::string positionLabel = std::string(std::string(iconPosition) + " Position");
        ImGui::Text(positionLabel.c_str());
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
    if (InspectorIndexUtility > -1 && InspectorIndex == -1)
    {
        if (interface::utilityType == interface::UTILITY_TYPE::light)
        {
            ImGui::Text("Name:");
            ImGui::SameLine();
            if (ImGui::Button("Delete"))
            {
                InspectorIndexUtility = -1; // Deselect the light after deletion
                entity.lights.erase(entity.lights.begin() + InspectorIndexUtility);
            }
            ImGui::Text(entity.lights[InspectorIndexUtility].name.c_str());
            ImGui::Separator();
            ImGui::Text("Position:");
            ImGui::DragFloat("X", &entity.lights[InspectorIndexUtility].position.x);
            ImGui::DragFloat("Y ", &entity.lights[InspectorIndexUtility].position.y);
            ImGui::DragFloat("Z  ", &entity.lights[InspectorIndexUtility].position.z);
            ImGui::Separator();
            ImGui::Text("Intensity: ");
            ImGui::DragFloat("Intensity  ", &entity.lights[InspectorIndexUtility].intensity);
            ImGui::Separator();
            ImGui::ColorEdit3("Color", &entity.lights[InspectorIndexUtility].lightColor.x);
        }
        else if (interface::utilityType == interface::UTILITY_TYPE::camera)
        {
            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::Text(cameras[InspectorIndexUtility].name.c_str());
            ImGui::Separator();
            ImGui::Separator();
            ImGui::Text("Position:");
            ImGui::DragFloat("X", &cameras[InspectorIndexUtility].cameraPos.x);
            ImGui::DragFloat("Y ", &cameras[InspectorIndexUtility].cameraPos.y);
            ImGui::DragFloat("Z  ", &cameras[InspectorIndexUtility].cameraPos.z);
            ImGui::Separator();
        }
        else
        {
            ImGui::Text("Failed to fetch backend");
        }
    }
    ImGui::End();
}