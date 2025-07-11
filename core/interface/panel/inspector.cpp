#include <core/components/components.hpp>
#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/model.hpp>
#include <utils/camera/camera.hpp>
#include <core/renderer/renderer.hpp>
#include <nlohmann/json.hpp>


using namespace QuavleEngine;

ObjectEntity entity;
Renderer renderForInspector;
ComponentsManager compMan;

void interface::inspector()
{
    ImGui::Begin("Inspector");
    compMan.load();

    if (selectedNames.empty())
    {
        if (InspectorIndex > -1)
        {
            std::string showLabel = std::string(std::string(iconShow) + " Show");
            ImGui::Checkbox(showLabel.c_str(), &entity.objects[InspectorIndex].isShow);

            ImGui::SameLine();

            std::string delateLabel = std::string(std::string(iconTrash) + " Delete");
            if (ImGui::Button(delateLabel.c_str()))
            {
                entity.objects.erase(entity.objects.begin() + InspectorIndex);
                InspectorIndex = -1;
                ImGui::End();
                return;
            }
            ImGui::Separator();

            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::Text(entity.objects[InspectorIndex].name.c_str());
            ImGui::Separator();

            if (ImGui::Button("copy UUID"))
            {
                ImGui::SetClipboardText(entity.objects[InspectorIndex].UUID.c_str());
            }
            ImGui::SameLine();
            ImGui::Text(entity.objects[InspectorIndex].UUID.c_str());

            ImGui::Separator();

            if (ImGui::CollapsingHeader("Object Transform"))
            {
                // Position
                std::string positionLabel = std::string(std::string(iconPosition) + " Position");
                ImGui::Text(positionLabel.c_str());
                ImGui::DragFloat("X", &entity.objects[InspectorIndex].position.x);
                ImGui::DragFloat("Y ", &entity.objects[InspectorIndex].position.y);  // Added space to differentiate label
                ImGui::DragFloat("Z  ", &entity.objects[InspectorIndex].position.z); // Added spaces to differentiate label
                ImGui::Separator();
                // Scale
                ImGui::Text("Scale");
                ImGui::DragFloat("X ", &entity.objects[InspectorIndex].scale.x);   // Added space to differentiate label
                ImGui::DragFloat("Y  ", &entity.objects[InspectorIndex].scale.y);  // Added spaces to differentiate label
                ImGui::DragFloat("Z   ", &entity.objects[InspectorIndex].scale.z); // Added spaces to differentiate label
                ImGui::Separator();

                // Rotation
                ImGui::Text("Rotation");
                ImGui::DragFloat("X    ", &entity.objects[InspectorIndex].rotation.x);   // Added spaces to differentiate label
                ImGui::DragFloat("Y     ", &entity.objects[InspectorIndex].rotation.y);  // Added spaces to differentiate label
                ImGui::DragFloat("Z      ", &entity.objects[InspectorIndex].rotation.z); // Added spaces to differentiate label
            }

            ImGui::Separator();

            if (!entity.objects[InspectorIndex].hasOwnTexture)
            {

                if (ImGui::CollapsingHeader("Material Data"))
                {
                    ImGui::ColorEdit3("Ambient", glm::value_ptr(entity.objects[InspectorIndex].material.ambient));
                    ImGui::ColorEdit3("Diffuse", glm::value_ptr(entity.objects[InspectorIndex].material.diffuse));
                    ImGui::ColorEdit3("Specular", glm::value_ptr(entity.objects[InspectorIndex].material.specular));
                    ImGui::ColorEdit3("Emissive", glm::value_ptr(entity.objects[InspectorIndex].material.emissive));

                    ImGui::SliderFloat("Shininess", &entity.objects[InspectorIndex].material.shininess, 0.0f, 256.0f);
                    ImGui::SliderFloat("Opacity", &entity.objects[InspectorIndex].material.opacity, 0.0f, 1.0f);
                    ImGui::SliderFloat("Shininess Strength", &entity.objects[InspectorIndex].material.shininessStrength, 0.0f, 10.0f);
                }

                ImGui::Separator();

                if (ImGui::CollapsingHeader("Material Texture"))
                {
                    const auto &obj = entity.objects[InspectorIndex];

                    auto ShowTextureSlot = [&](const char *label, GLuint textureID)
                    {
                        ImGui::Text("%s", label);
                        ImGui::SameLine();
                        ImGui::Image((void *)(intptr_t)textureID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
                         if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                        {
                            ImGui::BeginTooltip();
                            ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(300, 300));
                            ImGui::EndTooltip();
                        }
                        ImGui::Separator();
                    };

                    ShowTextureSlot("Diffuse", obj.diffuseTextureID);
                    ShowTextureSlot("Specular", obj.specularTextureID);
                    ShowTextureSlot("Normal", obj.normalTextureID);
                    ShowTextureSlot("Metallic", obj.metallicTextureID);
                    ShowTextureSlot("Roughness", obj.roughnessTextureID);
                    ShowTextureSlot("Height", obj.HeightTextureID);
                    ShowTextureSlot("Ambient", obj.ambientTextureID);
                    ShowTextureSlot("Emissive", obj.emissiveTextureID);
                    ShowTextureSlot("Opacity", obj.opacityTextureID);
                    ShowTextureSlot("Displacement", obj.displacementTextureID);
                }
            }
        }
        
        if (InspectorIndexUtility > -1)
        {
            if (interface::utilityType == interface::UTILITY_TYPE::light)
            {
                if (ImGui::Button("copy UUID"))
                {
                    ImGui::SetClipboardText(entity.lights[InspectorIndex].UUID.c_str());
                }

                ImGui::Separator();
                ImGui::Text("Name:");
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                    entity.lights.erase(entity.lights.begin() + InspectorIndexUtility);
                    InspectorIndexUtility = -1; // Deselect the light after deletion
                    ImGui::End();
                    return;
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

                if (ImGui::Button("Add components"))
                {
                    ImGui::OpenPopup("Comp List");
                }
                if (ImGui::BeginPopup("Comp List")){
                    ImGui::SeparatorText("Select Components");
                    for(int i = 0 ; i < sizeof(compMan.components) / sizeof(compMan.components[0]); i++){
                        ImGui::PushID(i + 2);
                        if(ImGui::Button(compMan.components[i]->getName())){
                            entity.lights[InspectorIndexUtility].components.push_back(i);
                        }
                        ImGui::PopID();
                    }
                    ImGui::Separator();
                    ImGui::EndPopup();
                }
                // Iterate through the components attached to the light and call their begin method
                for (int i = 0; i < entity.lights[InspectorIndexUtility].components.size(); i++)
                {
                    ImGui::PushID(i + 2 * 2);
                    int componentIndex = entity.lights[InspectorIndexUtility].components[i];
                    if (componentIndex >= 0 && componentIndex < sizeof(compMan.components) / sizeof(compMan.components[0]))
                        compMan.components[componentIndex]->begin(entity, InspectorIndexUtility);
                    ImGui::PopID();
                }
            }
            else if (interface::utilityType == interface::UTILITY_TYPE::camera)
            {
                // if (ImGui::Button("copy UUID"))
                // {
                //     ImGui::SetClipboardText(entity.lights[InspectorIndex].UUID.c_str());
                // }
                ImGui::Separator();
                ImGui::Text("Name:");
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                    cameras.erase(cameras.begin() + InspectorIndexUtility);
                    InspectorIndexUtility = -1; // Deselect the light after deletion
                    ImGui::End();
                    return;
                }
                ImGui::SameLine();
                ImGui::Text("Type:");
                ImGui::Separator();
                const char *cameraTypes[] = {"", "MASTER", "NONE"};
                int currentTypeIndex = static_cast<int>(cameras[InspectorIndexUtility].type);
                if (ImGui::Combo("##CameraType", &currentTypeIndex, cameraTypes, IM_ARRAYSIZE(cameraTypes)))
                {
                    cameras[InspectorIndexUtility].type = static_cast<CAMERATYPE>(currentTypeIndex);
                }
                ImGui::Separator();
                ImGui::Text(cameras[InspectorIndexUtility].name.c_str());
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Text("Position:");
                ImGui::DragFloat("X", &cameras[InspectorIndexUtility].cameraPos.x);
                ImGui::DragFloat("Y ", &cameras[InspectorIndexUtility].cameraPos.y);
                ImGui::DragFloat("Z  ", &cameras[InspectorIndexUtility].cameraPos.z);
                ImGui::Separator();
            }
            else if (interface::utilityType == interface::UTILITY_TYPE::billboard)
            {
                if (ImGui::Button("copy UUID"))
                {
                    ImGui::SetClipboardText(entity.billboards[InspectorIndex].UUID.c_str());
                }
                ImGui::SameLine();
                //ImGui::Text(entity.objects[InspectorIndex].UUID.c_str());
                ImGui::Separator();
                ImGui::Text("Name:");
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                    entity.billboards.erase(entity.billboards.begin() + InspectorIndexUtility);
                    InspectorIndexUtility = -1; // Deselect the light after deletion
                    ImGui::End();
                    return;
                }
                ImGui::Separator();
                ImGui::Checkbox("Look at", &entity.billboards[InspectorIndexUtility].lookAt);
                ImGui::Separator();
                ImGui::Text(entity.billboards[InspectorIndexUtility].name.c_str());
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Text("Position:");
                ImGui::DragFloat("X", &entity.billboards[InspectorIndexUtility].position.x);
                ImGui::DragFloat("Y ", &entity.billboards[InspectorIndexUtility].position.y);
                ImGui::DragFloat("Z  ", &entity.billboards[InspectorIndexUtility].position.z);
                ImGui::Separator();
                if (!entity.billboards[InspectorIndexUtility].lookAt)
                {
                    ImGui::Text("Rotation:");
                    ImGui::DragFloat("X ", &entity.billboards[InspectorIndexUtility].rotation.x);
                    ImGui::DragFloat("Y  ", &entity.billboards[InspectorIndexUtility].rotation.y);
                    ImGui::DragFloat("Z   ", &entity.billboards[InspectorIndexUtility].rotation.z);
                    ImGui::Separator();
                }
                ImGui::Separator();
                ImGui::Text("Texture:");
                if (ImGui::ImageButton("Texture", entity.billboards[InspectorIndexUtility].textureID, ImVec2(50, 50)))
                {
                }
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    ImGui::BeginTooltip();
                    ImGui::Image((ImTextureID)(intptr_t)entity.billboards[InspectorIndexUtility].textureID, ImVec2(100, 100));
                    ImGui::Text(entity.billboards[InspectorIndexUtility].texLocation.c_str());
                    ImGui::EndTooltip();
                }
                ImGui::Separator();
            }
            else
            {
                ImGui::Text("No item selected");
            }
        }
    }

    if (!selectedNames.empty() && interface::OBJECT_TYPE::multi)
    {
        if (ImGui::Button("Delate"))
        {
            for (int i = entity.objects.size() - 1; i >= 0; --i)
            {
                if (entity.objects[i].isSelected)
                    entity.objects.erase(entity.objects.begin() + i);
            }
        }
        if (ImGui::CollapsingHeader("Object Transform"))
        {
            if (selectedNames.size() > 1)
            {
                // Add this visibility toggle at the top of the transform section
                bool allVisible = std::all_of(selectedNames.begin(), selectedNames.end(),
                                              [&](const std::string &name)
                                              {
                                                  auto it = std::find_if(entity.objects.begin(), entity.objects.end(),
                                                                         [&](const auto &obj)
                                                                         { return obj.name == name; });
                                                  return it != entity.objects.end() && it->isShow;
                                              });

                if (ImGui::Checkbox("Visible", &allVisible))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                        {
                            obj.isShow = allVisible;
                        }
                    }
                }

                ImGui::Separator();

                // Show common properties that can be batch edited
                static float commonPosition[3] = {0};
                static float commonScale[3] = {1, 1, 1};
                static float commonRotation[3] = {0};
                // Position
                std::string positionLabel = std::string(std::string(iconPosition) + " Position");
                ImGui::Text(positionLabel.c_str());
                if (ImGui::DragFloat("X", &commonPosition[0]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.position.x = commonPosition[0];
                    }
                }
                if (ImGui::DragFloat("Y ", &commonPosition[1]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.position.y = commonPosition[1];
                    }
                }
                if (ImGui::DragFloat("Z  ", &commonPosition[2]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.position.z = commonPosition[2];
                    }
                }
                ImGui::Separator();

                // Scale
                ImGui::Text("Scale");
                if (ImGui::DragFloat("X ", &commonScale[0]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.scale.x = commonScale[0];
                    }
                }
                if (ImGui::DragFloat("Y  ", &commonScale[1]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.scale.y = commonScale[1];
                    }
                }
                if (ImGui::DragFloat("Z   ", &commonScale[2]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.scale.z = commonScale[2];
                    }
                }
                ImGui::Separator();

                // Rotation
                ImGui::Text("Rotation");
                if (ImGui::DragFloat("X    ", &commonRotation[0]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.rotation.x = commonRotation[0];
                    }
                }
                if (ImGui::DragFloat("Y     ", &commonRotation[1]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.rotation.y = commonRotation[1];
                    }
                }
                if (ImGui::DragFloat("Z      ", &commonRotation[2]))
                {
                    for (auto &obj : entity.objects)
                    {
                        if (obj.isSelected)
                            obj.rotation.z = commonRotation[2];
                    }
                }
            }
        }
    }

    ImGui::End();
}