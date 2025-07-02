#include <core/interface/interface.hpp>

using namespace QuavleEngine;
std::vector<QuavleEngine::interface::messageDebug> QuavleEngine::interface::message;

void interface::consolePanel()
{
    int displayedRow = 0;
    static char filterText[128] = "";
    static int typeFilter = 0; // 0: All, 1: Info, 2: Debug, 3: Warning

    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // Filter Inputs
    ImGui::InputTextWithHint("##filterText", "Filter by text...", filterText, IM_ARRAYSIZE(filterText));
    ImGui::SameLine();
    ImGui::Text("Type:");
    ImGui::SameLine();
    ImGui::Combo("##typeFilter", &typeFilter, "All\0Info\0Debug\0Warning\0");

    ImGui::Separator();


    // Table headers: Type | Message
    if (ImGui::BeginTable("ConsoleTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Message");
        ImGui::TableHeadersRow();

        for (const auto &msg : message)
        {
            // Filter by type
            bool typeMatch = (typeFilter == 0) ||
                             (typeFilter == 1 && msg.type == "Info") ||
                             (typeFilter == 2 && msg.type == "Debug") ||
                             (typeFilter == 3 && msg.type == "Warning");

            // Filter by text
            bool textMatch = std::string(msg.text).find(filterText) != std::string::npos;

            if (!typeMatch || !textMatch)
                continue;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(std::to_string(displayedRow++).c_str());

            ImGui::TableSetColumnIndex(1);

            ImVec4 color;
            if (msg.type == "Warning")
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            else if (msg.type == "Debug")
                color = ImVec4(236.0f / 255.0f, 1.0f, 0.0f, 1.0f);
            else if (msg.type == "Info")
                color = ImVec4(0.0f, 1.0f, 29.0f / 255.0f, 1.0f);
            else
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            ImGui::TextColored(color, "%s", msg.type.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::TextUnformatted(msg.text.c_str());
        }

        ImGui::EndTable();
    }
    ImGui::Text("Message %zu", message.size());

    ImGui::End();
}

void interface::inputDebug(const std::string& typeOfDebug, const std::string& text)
{
    messageDebug newMessage;
    newMessage.text = text;
    newMessage.type = typeOfDebug;
    message.push_back(newMessage);
}
