#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/model.hpp>
#include <utils/camera/camera.hpp>
#include <core/renderer/renderer.hpp>
#include <nlohmann/json.hpp>


using namespace QuavleEngine;

ObjectEntity entity;
Renderer renderForInspector;

void shaderVariableViewer(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        return;
    }

    nlohmann::json j;
    in >> j;

    if (j.contains("nodes") && j["nodes"].is_array())
    {
        for (const auto &node : j["nodes"])
        {
            if (!node.contains("value") || !node["value"].contains("valueNode"))
                continue;

            const auto &valueNode = node["value"]["valueNode"];
            std::string label = node.contains("title") ? node["title"].get<std::string>() : "Node";

            if (valueNode.is_array() && valueNode.size() == 3)
            {
                // It's a vec3
                glm::vec3 val(valueNode[0], valueNode[1], valueNode[2]);
                ImGui::Text("%s Value (Vec3):", label.c_str());
                ImGui::DragFloat3(("##" + label + "_vec3").c_str(), glm::value_ptr(val));
            }
            else if (valueNode.is_number())
            {
                // It's a float
                float val = valueNode.get<float>();
                ImGui::Text("%s Value (Float):", label.c_str());
                ImGui::DragFloat(("##" + label + "_float").c_str(), &val);
            }
            else
            {
                ImGui::Text("%s Value: %s", label.c_str(), valueNode.dump().c_str());
            }
        }
    }
    else
    {
        ImGui::Text("Node Value: N/A (or not found)");
    }
}

void interface::inspector()
{
    ImGui::Begin("Inspector");

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

            if (!entity.objects[InspectorIndex].hasOwnTexture)
            {
                ImGui::Separator();
                if (ImGui::Button("Create new shader"))
                {
                }
            }else{
                if (ImGui::Button(("Open "+entity.objects[InspectorIndex].name+".shd").c_str()))
                {

                }
            }
        }
        
        if (InspectorIndexUtility > -1)
        {
            if (interface::utilityType == interface::UTILITY_TYPE::light)
            {
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
    }

    if (!selectedNames.empty())
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