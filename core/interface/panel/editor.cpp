#include <core/interface/interface.hpp>
#include <TextEditor.h>
#include <fstream>
#include <string>
#include <utils/config.hpp>

using namespace QuavleEngine;
TextEditor editor;

std::string scriptPath = config["LOCATION"]["script"];

void interface::codeEditor()
{
    std::string currentFile = codePath;
    static std::filesystem::file_time_type lastWriteTime;
    static bool shouldReload = false;

    // Set Lua language highlighting once
    static bool initialized = false;
    if (!initialized)
    {
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        editor.SetPalette(TextEditor::GetDarkPalette()); // Optional: Use a nicer color scheme
        initialized = true;
    }

    // Check for external modification
    if (std::filesystem::exists(currentFile))
    {
        auto writeTime = std::filesystem::last_write_time(currentFile);
        if (writeTime != lastWriteTime)
        {
            shouldReload = true;
            lastWriteTime = writeTime;
        }
    }

    ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

    // === Menu Bar ===
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                editor.SetText("");
                currentFile = "untitled.lua";
                codePath.clear();
            }

            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                std::ifstream file(currentFile);
                if (file)
                {
                    std::string content((std::istreambuf_iterator<char>(file)), {});
                    editor.SetText(content);
                    lastWriteTime = std::filesystem::last_write_time(currentFile);
                }
            }

            if (ImGui::MenuItem("Save", "Ctrl+S", false, editor.IsTextChanged()))
            {
                std::ofstream file(currentFile);
                if (file)
                {
                    file << editor.GetText();
                    lastWriteTime = std::filesystem::last_write_time(currentFile);
                }
            }

            if (ImGui::MenuItem("Quit", "Ctrl+O"))
            {
                isCodeEditor = false;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, editor.CanUndo())) editor.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, editor.CanRedo())) editor.Redo();
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) editor.Cut();
            if (ImGui::MenuItem("Copy", "Ctrl+C")) editor.Copy();
            if (ImGui::MenuItem("Paste", "Ctrl+V")) editor.Paste();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // === Reload warning ===
    if (shouldReload)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
        ImGui::Text("File modified externally. Press Ctrl+R to reload.");
        ImGui::PopStyleColor();

        if (ImGui::IsKeyPressed(ImGuiKey_R) && ImGui::GetIO().KeyCtrl)
        {
            std::ifstream file(currentFile);
            if (file)
            {
                std::string content((std::istreambuf_iterator<char>(file)), {});
                editor.SetText(content);
                shouldReload = false;
                lastWriteTime = std::filesystem::last_write_time(currentFile);
            }
        }
    }

    // === Render editor ===
    editor.Render(currentFile.c_str());

    // === Status bar ===
    ImGui::Separator();
    auto cursor = editor.GetCursorPosition();
    ImGui::Text("Line %d, Col %d", cursor.mLine + 1, cursor.mColumn + 1);
    ImGui::SameLine();
    ImGui::TextDisabled("|");
    ImGui::SameLine();
    ImGui::Text("%s", editor.IsTextChanged() ? "Modified" : "Saved");

    ImGui::End();

    // === Keyboard shortcuts ===
    ImGuiIO &io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N))
        editor.SetText("");

    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))
    {
        std::ofstream file(currentFile);
        if (file)
        {
            file << editor.GetText();
            lastWriteTime = std::filesystem::last_write_time(currentFile);
        }
    }

    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O))
    {
        std::ifstream file(currentFile);
        if (file)
        {
            std::string content((std::istreambuf_iterator<char>(file)), {});
            editor.SetText(content);
            lastWriteTime = std::filesystem::last_write_time(currentFile);
        }
    }
}


void interface::loadCode(const std::string& path)
{
    codePath = path;

    std::ifstream file(path);
    if (file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), {});
        editor.SetText(content);
        // Optional: set Lua again
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    }
}
