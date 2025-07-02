#define IMGUI_DEFINE_MATH_OPERATOR
#include <core/interface/interface.hpp>
#include <core/interface/nodes.h>

using namespace QuavleEngine;

void interface::nodePanel()
{
    static NodeEditor* neditor = nullptr;
    ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_NoCollapse);
    ImVec2 editor_size = interface::m_viewportSize;
    if (!neditor) {
        neditor = new NodeEditor((int)editor_size.x, (int)editor_size.y);
    }
    neditor->set_size(editor_size);
    neditor->draw();
    ImGui::End();
}