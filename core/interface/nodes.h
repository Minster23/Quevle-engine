#pragma once
#define IMGUI_DEFINE_MATH_OPERATOR
#include "ImNodeFlow.h"
#include <core/interface/interface.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utils/config.hpp>
#include <nlohmann/json.hpp>

extern ImFlow::ImNodeFlow mINF;
extern nlohmann::json j;

extern std::vector<std::string> nodesList;
extern int indexNodeSelector;

extern std::string shaderPath;
extern bool nodePanelOpen;

void exportMaterialToShader(
    const glm::vec3 &baseColor,
    float opacity,
    float metallic,
    float roughness,
    const std::string &filepath);

// * MATERIAL NODE
class ColorNode : public ImFlow::BaseNode
{
public:
    ColorNode()
    {
        setTitle("Color");
        ImFlow::BaseNode::addOUT<glm::vec3>("RGB", nullptr)->behaviour([this]()
                                                                       { return color; });
    }

    void draw() override
    {
        ImGui::SetNextItemWidth(100.f);
        ImGui::ColorEdit3("Color", &color[0]);
    }

    void setValue(glm::vec3 value)
    {
        color = value;
    }

    void setTitle(std::string title)
    {
        setTitle(title);
    }

private:
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); // default: white
};

//

class VariableNode : public ImFlow::BaseNode
{
public:
    VariableNode()
    {
        setTitle("Float");
        ImFlow::BaseNode::addOUT<float>("Out", nullptr)->behaviour([this]()
                                                                   { return valueFloat; });
    }

    void draw() override
    {
        ImGui::SetNextItemWidth(100.f);
        ImGui::DragFloat("Value", &valueFloat, 0.1f);
    }

    void updateNodeType()
    {
        setTitle("Float: " + std::to_string(valueFloat));
    }

    void setValue(float value)
    {
        valueFloat = value;
        updateNodeType();
    }

    void setTitle(std::string title)
    {
        setTitle(title);
    }


private:
    float valueFloat = 0;
};

//

class MultiplyFloat : public ImFlow::BaseNode
{
public:
    MultiplyFloat()
    {
        setTitle("Multiply (float)");
        ImFlow::BaseNode::addIN<float>("A", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<float>("B", 1, ImFlow::ConnectionFilter::SameType());

        ImFlow::BaseNode::addOUT<float>("Result", nullptr)->behaviour([this]()
                                                                      { return result; });
    }

    void draw() override
    {
        float a = getInVal<float>("A");
        float b = getInVal<float>("B");
        result = a * b;

        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%.3f", result);
    }

private:
    float result = 0.0f;
};

//

class SubtractVec3 : public ImFlow::BaseNode
{
public:
    SubtractVec3()
    {
        setTitle("Subtract (vec3)");
        ImFlow::BaseNode::addIN<glm::vec3>("A", glm::vec3(0.0f), ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<glm::vec3>("B", glm::vec3(0.0f), ImFlow::ConnectionFilter::SameType());

        ImFlow::BaseNode::addOUT<glm::vec3>("Result", nullptr)->behaviour([this]()
                                                                          { return result; });
    }

    void draw() override
    {
        glm::vec3 a = getInVal<glm::vec3>("A");
        glm::vec3 b = getInVal<glm::vec3>("B");
        result = a - b;

        ImGui::SetNextItemWidth(100.f);
        ImGui::Checkbox("Text", &Text);

        if (Text)
        {
            ImGui::Text("%s", glm::to_string(result).c_str());
        }
        else
        {
            ImGui::ColorButton("Color", ImVec4(result.r, result.g, result.b, 1), 0, ImVec2(60, 60));
        }
    }

private:
    glm::vec3 result = glm::vec3(0.0f);
    bool Text = false;
};

//! DEFAULT
class OutputNode : public ImFlow::BaseNode
{
public:
    OutputNode()
    {
        setTitle("Fragment Output");
        setStyle(ImFlow::NodeStyle::green());
        ImFlow::BaseNode::addIN<glm::vec3>("Base Color", glm::vec3(0.0f), ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<float>("Opacity", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<float>("Metallic", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<float>("Roughness", 0, ImFlow::ConnectionFilter::SameType());
    }

    void draw() override
    {
        baseColor = getInVal<glm::vec3>("Base Color");
        opacity = getInVal<float>("Opacity");
        metallic = getInVal<float>("Metallic");
        roughness = getInVal<float>("Roughness");

        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("Final Output Preview:");
        ImGui::ColorButton("Color", ImVec4(baseColor.r, baseColor.g, baseColor.b, opacity), 0, ImVec2(60, 60));
        ImGui::Text("Metallic: %.2f", metallic);
        ImGui::Text("Roughness: %.2f", roughness);
    }

    glm::vec3 baseColor;
    float opacity;
    float metallic;
    float roughness;
};


// Simple function to connect ColorNode to OutputNode
inline void connectColorToOutput(std::shared_ptr<ColorNode> colorNode, std::shared_ptr<OutputNode> outputNode)
{
    // Find RGB output pin from ColorNode
    for (auto &pin : colorNode->getOuts()) {
        std::cout << "Color Pin name: " << pin->getName() << std::endl;
        if (pin->getName() == "RGB") {
            // Find Base Color input pin from OutputNode
            for (auto &inPin : outputNode->getIns()) {
                std::cout << "inputPin name in output: " << inPin->getName() << std::endl;
                if (inPin->getName() == "Base Color") {
                    std::cout<<"Linked node" + colorNode->getName() + " to " + outputNode->getName() <<std::endl;
                    pin->createLink(inPin.get());
                    return;
                }
            }
        }
    }
}

struct NodeEditor : ImFlow::BaseNode
{

    NodeEditor(float d, std::size_t r) : BaseNode()
    {
        mINF.setSize({d, d});
        if (r > 0)
        {
            // Create nodes
            auto colorNode = mINF.addNode<ColorNode>({0, 0});
            auto outputNode = mINF.addNode<OutputNode>({200, 0});

            // Connect them
            connectColorToOutput(colorNode, outputNode);
        }
    }

    void set_size(ImVec2 d)
    {
        mINF.setSize(d);
    }

    OutputNode *getOutputNode()
    {
        // Find the OutputNode in mINF's node list
        for (auto &node : mINF.getNodes()) // Fixed: Changed 'node.get()' to 'node.second.get()'
        {
            if (auto out = dynamic_cast<OutputNode *>(node.second.get()))
            {
                return out;
            }
        }
        return nullptr;
    }

    void draw() override
    {
        mINF.update();

        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();

        bool mouseInWindow = mousePos.x >= winPos.x && mousePos.x <= winPos.x + winSize.x &&
                             mousePos.y >= winPos.y && mousePos.y <= winPos.y + winSize.y;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered() && mouseInWindow)
        {
            ImGui::OpenPopup("Dialog");
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
        }

        // Popup content
        if (ImGui::BeginPopup("Dialog"))
        {
            ImGui::Text("Choose");
            ImGui::Separator();
            if (nodesList.size() == 0)
            {
                ImGui::Text("No Nodes");
            }
            else
            {
                for (int i = 0; i < nodesList.size(); i++)
                {
                    if (ImGui::Button(nodesList[i].c_str()))
                    {
                        indexNodeSelector = i;

                        if (indexNodeSelector == 0)
                        {
                            mINF.placeNodeAt<VariableNode>({mousePos.x, mousePos.y});
                        }
                        else if (indexNodeSelector == 1)
                        {
                            mINF.placeNodeAt<ColorNode>({mousePos.x, mousePos.y});
                        }
                        else if (indexNodeSelector == 2)
                        {
                            mINF.placeNodeAt<MultiplyFloat>({mousePos.x, mousePos.y});
                        }
                        else if (indexNodeSelector == 3)
                        {
                            mINF.placeNodeAt<SubtractVec3>({mousePos.x, mousePos.y});
                        }
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::EndPopup();
        }
    }
};

void starterEditorOpen();

void save(const std::string& filename);

void load(const std::string& filename);

void openEditor(const std::string& filename);

void closeEditor();