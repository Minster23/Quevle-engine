#include <core/interface/interface.hpp>

using namespace QuavleEngine;
std::vector<QuavleEngine::interface::messageDebug> QuavleEngine::interface::message;

void interface::consolePanel()
{
    static char filterText[128] = "";
    static int typeFilter = 0; // 0: All, 1: Info, 2: Debug, 3: Warning
    static bool autoScroll = true;

    ImGui::Begin("Console");

    // Filter bar
    ImGui::InputTextWithHint("##Filter", "Filter by text...", filterText, IM_ARRAYSIZE(filterText));
    ImGui::SameLine();
    ImGui::Text("Type:");
    ImGui::SameLine();
    ImGui::Combo("##TypeFilter", &typeFilter, "All\0Info\0Debug\0Warning\0");

    ImGui::SameLine();
    if (ImGui::Button("Clear Log")) {
        message.clear();
    }

    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::Separator();

    // Log area
    ImGui::BeginChild("ConsoleLogArea", ImVec2(0, 400), true, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTable("ConsoleTable", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("No", ImGuiTableColumnFlags_WidthFixed, 40.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Message");
        ImGui::TableHeadersRow();

        int index = 0;
        for (const auto &msg : message)
        {
            // Filter by type
            bool typeMatch = (typeFilter == 0) ||
                             (typeFilter == 1 && msg.type == "Info") ||
                             (typeFilter == 2 && msg.type == "Debug") ||
                             (typeFilter == 3 && msg.type == "Warning");

            // Filter by text
            bool textMatch = std::string(msg.text).find(filterText) != std::string::npos;

            if (!typeMatch || !textMatch) continue;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", index++);

            ImGui::TableSetColumnIndex(1);
            ImVec4 color;
            if (msg.type == "Warning")
                color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            else if (msg.type == "Debug")
                color = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
            else if (msg.type == "Info")
                color = ImVec4(0.4f, 0.9f, 0.4f, 1.0f);
            else
                color = ImVec4(1, 1, 1, 1);

            ImGui::TextColored(color, "%s", msg.type.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::TextWrapped("%s", msg.text.c_str());
        }

        ImGui::EndTable();

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f); // scroll to bottom
    }
    ImGui::EndChild();

    ImGui::Text("Total messages: %zu", message.size());
    ImGui::End();
}


void interface::inputDebug(const std::string& typeOfDebug, const std::string& text)
{
    messageDebug newMessage;
    newMessage.text = text;
    newMessage.type = typeOfDebug;
    message.push_back(newMessage);
}
