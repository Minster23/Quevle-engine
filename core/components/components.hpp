#pragma once

#include <core/renderer/entity/objectEntity.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <core/interface/interface.hpp>
#include <optional>
#include <core/scripting/scripting.h>

namespace QuavleEngine
{
    enum componentsType
    {
        LIGHT,
        BILLBOARD,
        OBJECT
    };

    class ComponentsManager
    {

    public:
        void load();
        ComponentsManager *components[2];

        virtual const char* getName() const {return "ComponentsManager";}
        virtual void begin(std::optional<ObjectEntity> obj, int index, componentsType type){}
        virtual ~ComponentsManager() {}
    };

    class light_setting : public ComponentsManager
    {
    public:
        const char* getName() const override { return "lightSetting"; }
        void begin(std::optional<ObjectEntity> obj, int index, componentsType type) override
        {
            if (obj && type == componentsType::LIGHT)
            {
                ImGui::SeparatorText("My Object");
                ImGui::Text("Its components");
                ImGui::Checkbox("Show?", &obj->lights[index].isShow);
                ImGui::Separator();
            }else{
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No object selected or wrong type!");
            }
        }
    };

    class create_script : public ComponentsManager
    {
    public:
        const char *getName() const override { return "create script"; }
        void begin(std::optional<ObjectEntity> obj, int index, componentsType type) override
        {
            static bool showScriptInputPopup = false;
            static char scriptNameBuffer[256] = "";

            if (obj && type == componentsType::OBJECT)
            {
                ImGui::SeparatorText("My Object");
                if(ImGui::Button("Create Script"))
                {
                    std::cout << "Creating script for object: " << obj->objects[index].name << std::endl;
                    showScriptInputPopup = true;
                    strcpy(scriptNameBuffer, ""); // Clear buffer for new input
                }

                if (showScriptInputPopup)
                {
                    ImGui::OpenPopup("Enter Script Name");
                }

                if (ImGui::BeginPopupModal("Enter Script Name", &showScriptInputPopup, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::InputText("Script Name", scriptNameBuffer, IM_ARRAYSIZE(scriptNameBuffer));
                    if (ImGui::Button("Create") && strlen(scriptNameBuffer) > 0)
                    {
                        Scripting::script scriptManager;
                        scriptManager.newScript(scriptNameBuffer, index);
                        std::cout << "Script name entered: " << scriptNameBuffer << std::endl;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                // Display existing script locations
                if (!obj->objects[index].scriptLocation.empty()) {
                    ImGui::Text("Attached Scripts:");
                    for (const auto& scriptPath : obj->objects[index].scriptLocation) {
                        ImGui::Text("  - %s", scriptPath.c_str());
                    }
                } else {
                    ImGui::Text("No scripts attached.");
                }
                ImGui::Separator();
            }
            else
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No object selected or wrong type!");
            }
        }
    };
}