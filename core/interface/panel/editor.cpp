#include <core/interface/interface.hpp>
#include <TextEditor.h>
#include <fstream>
#include <string>

using namespace QuavleEngine;
TextEditor editor;

void interface::codeEditor()
{
    std::string currentFile = codePath;
    static std::filesystem::file_time_type lastWriteTime;
    static bool shouldReload = false;

    // If no file, just return or show a "No file" window
    if (currentFile.empty())
        return;

    // Set Lua syntax highlighting once
    static bool initialized = false;
    if (!initialized)
    {
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        initialized = true;
    }

    // Check for external file modification
    if (std::filesystem::exists(currentFile))
    {
        auto writeTime = std::filesystem::last_write_time(currentFile);
        if (writeTime != lastWriteTime)
        {
            shouldReload = true;
            lastWriteTime = writeTime;
        }
    }

    ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

    // Menu bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                editor.SetText("");
                currentFile = "untitled.lua";
            }

            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                std::ifstream file(currentFile);
                if (file)
                {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());
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

            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
            {
                // Dumb Save As: change to another fixed file name
                currentFile = "saved_as.lua";
                std::ofstream file(currentFile);
                if (file)
                {
                    file << editor.GetText();
                    lastWriteTime = std::filesystem::last_write_time(currentFile);
                }
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

    // Show reload suggestion
    if (shouldReload)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "File modified externally. Press Ctrl+R to reload.");
        if (ImGui::IsKeyPressed(ImGuiKey_R) && (ImGui::GetIO().KeyCtrl))
        {
            std::ifstream file(currentFile);
            if (file)
            {
                std::string content((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
                editor.SetText(content);
                shouldReload = false;
                lastWriteTime = std::filesystem::last_write_time(currentFile);
            }
        }
    }

    // Render editor
    editor.Render(currentFile.c_str());

    // Footer: status bar
    ImGui::Separator();
    auto cursor = editor.GetCursorPosition();
    ImGui::Text("Line %d, Col %d  |  %s", cursor.mLine + 1, cursor.mColumn + 1,
                editor.IsTextChanged() ? "Modified" : "Saved");

    ImGui::End();
}


void interface::loadCode(std::string path)
{
    codePath = path;
}
