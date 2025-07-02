#pragma once
#define IMGUI_DEFINE_MATH_OPERATOR
#include "ImNodeFlow.h"
#include <core/interface/interface.hpp>
#include <iostream>

QuavleEngine::interface interfaceForNode;
class StartSum : public ImFlow::BaseNode
{
public:
    StartSum()
    {
        setTitle("Start");
        setStyle(ImFlow::NodeStyle::green());
        ImFlow::BaseNode::addIN<int>("In", 0, ImFlow::ConnectionFilter::SameType()); // Add this line
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return getInVal<int>("In") + m_valA; });
    }

    void draw() override
    {
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputInt("##ValB", &m_valA);
    }

private:
    int m_valA = 0;
};

class Variable : public ImFlow::BaseNode
{
public:
    Variable()
    {
        setTitle("Variable");
        setStyle(ImFlow::NodeStyle::red());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return variable; });
    }

    void draw() override
    {
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputInt("##ValB", &variable);
    }

private:
    int variable = 0;
};

class FinishSum : public ImFlow::BaseNode
{
public:
    FinishSum()
    {
        setTitle("Finish");
        setStyle(ImFlow::NodeStyle::cyan());
        ImFlow::BaseNode::addIN<int>("In", 0, ImFlow::ConnectionFilter::SameType());
    }

    void draw() override
    {
        ImGui::SetNextItemWidth(100.f);
        int inVal = ImFlow::BaseNode::getInVal<int>("In");
        ImGui::Text("%llu", inVal);
        if (inVal != lastInValue) {
            interfaceForNode.inputDebug("Info", std::to_string(inVal) + " from nodes");
            lastInValue = inVal;
        }
    }

private:
    int m_valB = 0;
    int lastInValue = 0; // Add this line
};

//* ===== OPRATOR ========

class opratorPlus : public ImFlow::BaseNode
{
public:
    opratorPlus()
    {
        setTitle("+");
        ImFlow::BaseNode::addIN<int>("In 1", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("In 2", 1, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return totalSum; });
    }

    void draw() override
    {
        totalSum = getInVal<int>("In 1") + getInVal<int>("In 2");
        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%d", totalSum);
    }

private:
    int totalSum = 0;
};

class opratorMins : public ImFlow::BaseNode
{
public:
    opratorMins()
    {
        setTitle("-");
        ImFlow::BaseNode::addIN<int>("In 1", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("In 2", 1, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return totalSum; });
    }

    void draw() override
    {
        totalSum = getInVal<int>("In 1") - getInVal<int>("In 2");
        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%d", totalSum);
    }

private:
    int totalSum = 0;
};

class opratorMul : public ImFlow::BaseNode
{
public:
    opratorMul()
    {
        setTitle("*");
        ImFlow::BaseNode::addIN<int>("In 1", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("In 2", 1, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return total; });
    }

    void draw() override
    {
        total = getInVal<int>("In 1") * getInVal<int>("In 2");
        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%d", total);
    }

private:
    int total = 0;
};

class opratorDiv : public ImFlow::BaseNode
{
public:
    opratorDiv()
    {
        setTitle("/");
        ImFlow::BaseNode::addIN<int>("In 1", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("In 2", 1, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return result; });
    }

    void draw() override
    {
        int a = getInVal<int>("In 1");
        int b = getInVal<int>("In 2");
        result = (b != 0) ? (a / b) : 0;
        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%d", result);
    }

private:
    int result = 0;
};

class opratorMod : public ImFlow::BaseNode
{
public:
    opratorMod()
    {
        setTitle("%");
        ImFlow::BaseNode::addIN<int>("In 1", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("In 2", 1, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]()
                                                                 { return result; });
    }

    void draw() override
    {
        int a = getInVal<int>("In 1");
        int b = getInVal<int>("In 2");
        result = (b != 0) ? (a % b) : 0;
        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("%d", result);
    }

private:
    int result = 0;
};

struct NodeEditor : ImFlow::BaseNode
{
    ImFlow::ImNodeFlow mINF;

    NodeEditor(float d, std::size_t r) : BaseNode()
    {
        mINF.setSize({d, d});
        if (r > 0)
        {
            mINF.addNode<StartSum>({0, 0});
            mINF.addNode<Variable>({10, 0});
            mINF.addNode<Variable>({10, 0});
            mINF.addNode<Variable>({10, 0});
            mINF.addNode<Variable>({10, 0});
            mINF.addNode<FinishSum>({10, 10});
        }
    }

    void set_size(ImVec2 d)
    {
        mINF.setSize(d);
    }

    void draw() override
    {
        mINF.update();
    }
};
