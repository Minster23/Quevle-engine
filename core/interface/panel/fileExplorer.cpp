#include <core/interface/interface.hpp>
#include <imgui_internal.h>
#include <core/renderer/renderer.hpp>
#include <algorithm>


using namespace QuavleEngine;
Renderer renderForFileExplorer;


void interface::fileExplorer()
{

    ImGui::Begin("Files", nullptr, ImGuiWindowFlags_MenuBar);
    ImGuiID dockspace_id = ImGui::GetID("FILES_Dockspace");
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::DockSpace(dockspace_id, size, ImGuiDockNodeFlags_None);
    ImGui::End();

    ImGui::Begin("Folder");
    // Back button to go to the parent directory
    if (std::filesystem::path(currentDirectory).has_parent_path())
    {
        if (ImGui::Button(ICON_CI_ARROW_UP " .."))
        {
            currentDirectory = std::filesystem::path(currentDirectory).parent_path().string();
        }
    }
    ImGui::Separator();
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(currentDirectory))
        {
            if (entry.is_directory())
            {
                std::string buttonLabel = ICON_CI_FOLDER " " + entry.path().filename().string();
                if (ImGui::Button(buttonLabel.c_str()))
                {
                    currentDirectory = entry.path().string();
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        ImGui::TextColored({1.f, 0.f, 0.f, 1.f}, "Error: %s", e.what());
    }
    ImGui::End();

    ImGui::Begin("File Explorer");

    float itemWidth = 100.0f;
    float itemHeight = 100.0f; // Make it square like Unity's grid icons
    float textHeight = ImGui::GetFontSize() + ImGui::GetStyle().ItemSpacing.y;

    int numColumns = static_cast<int>(ImGui::GetContentRegionAvail().x / itemWidth);
    if (numColumns < 1)
        numColumns = 1;

    if (ImGui::BeginTable("FileGrid", numColumns, ImGuiTableFlags_SizingStretchSame))
    {
        try
        {
            for (const auto &entry : std::filesystem::directory_iterator(currentDirectory))
            {
                if (!entry.is_directory())
                {
                    ImGui::TableNextColumn();

                    const auto &path = entry.path();
                    std::string extension = path.extension().string();

                    const char *icon = ICON_CI_FILE;
                    if (extension == ".obj" || extension == ".gltf" || extension == ".fbx")
                        icon = ICON_CI_SYMBOL_OBJECT;
                    else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                        icon = ICON_CI_FILE_MEDIA;
                    else if (extension == ".cpp" || extension == ".h" || extension == ".hpp" || extension == ".lua")
                        icon = ICON_CI_FILE_CODE;

                    std::string filename = path.filename().string();
                    std::string shortName = filename;
                    if (shortName.size() > 15)
                    {
                        shortName = shortName.substr(0, 10) + "..." + path.extension().string();
                    }

                    ImGui::BeginGroup();

                    ImVec2 cellSize(itemWidth, itemHeight);
                    ImVec2 iconSize = ImGui::CalcTextSize(icon);
                    ImVec2 textSize = ImGui::CalcTextSize(shortName.c_str());

                    float totalHeight = iconSize.y + ImGui::GetStyle().ItemSpacing.y + textSize.y;

                    // Center vertically
                    float verticalOffset = (cellSize.y - totalHeight) * 0.5f;

                    // Move down to start position
                    ImGui::Dummy(ImVec2(0.0f, verticalOffset));

                    // === Centered Icon ===
                    float iconX = (cellSize.x - iconSize.x) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconX);
                    ImGui::TextUnformatted(icon);

                    // === Centered Filename ===
                    float textX = (cellSize.x - textSize.x) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textX);
                    ImGui::TextWrapped("%s", shortName.c_str());

                    // === Clickable Area ===
                    ImRect bb(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
                    bb.Min.y -= iconSize.y + verticalOffset + ImGui::GetStyle().ItemSpacing.y;
                    bb.Max.y += textSize.y;
                    if (ImGui::IsMouseHoveringRect(bb.Min, bb.Max))
                    {
                        ImGui::SetTooltip("%s", path.string().c_str());
                        if (ImGui::IsMouseClicked(0))
                        {
                            if (extension == ".obj" || extension == ".gltf")
                            {
                                std::cout << "Openable\n";
                                std::string path_str = path.string();
                                std::replace(path_str.begin(), path_str.end(), '\\', '/');
                                inputDebug("Info", "loading model" + path_str);
                                renderForFileExplorer.loadModelFirst(path_str);
                            }else if(extension == ".lua"){
                                std::string path_str = path.string();
                                std::replace(path_str.begin(), path_str.end(), '\\', '/');
                                loadCode(path_str);
                            }
                            else{
                                inputDebug("Info", "opening file" + path.string());
                            }
                        }
                    }

                    ImGui::EndGroup();
                }
            }
        }
        catch (const std::filesystem::filesystem_error &)
        {
            // Ignore error
        }
        ImGui::EndTable();
    }

    ImGui::End();
}
