#pragma once

#include <core/renderer/entity/objectEntity.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <core/interface/interface.hpp>
#include <optional>

namespace QuavleEngine
{
    class ComponentsManager
    {

    public:
        void load();
        ComponentsManager *components[2];

        virtual const char* getName() const {return "ComponentsManager";}
        virtual void begin(std::optional<ObjectEntity> obj, int index){}
        virtual ~ComponentsManager() {}
    };

    class lightSetting : public ComponentsManager
    {
    public:
        const char* getName() const override { return "lightSetting"; }
        void begin(std::optional<ObjectEntity> obj, int index) override
        {
            
            if (obj)
            {
                ImGui::SeparatorText("My Object");
                ImGui::Text("Its components");
                ImGui::Checkbox("Show?", &obj->lights[index].isShow);
                ImGui::Separator();
            }
        }
    };

    class lightSettting : public ComponentsManager
    {
    public:
        const char *getName() const override { return "lightSetting"; }
        void begin(std::optional<ObjectEntity> obj, int index) override
        {
            if (obj)
            {
                ImGui::SeparatorText("My Object");
                ImGui::Text("Its components");
                ImGui::Checkbox("Show?", &obj->lights[index].isShow);
                ImGui::Separator();
            }
        }
    };
}