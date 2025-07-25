#include <core/interface/interface.hpp>
#include <imgui_internal.h>
#include <core/renderer/renderer.hpp>
#include <algorithm>
#include <utils/uiTextureloader/utiTexLoader.hpp>

using namespace QuavleEngine;
Renderer renderForFileExplorer;
uiTexLoader texLoader;
std::unordered_map<std::string, unsigned int> imageCache;

static std::string currentPath;
static std::string newName;

void interface::fileExplorer()
{
    ImGui::Begin("Files", nullptr);
    ImGuiID dockspace_id = ImGui::GetID("FILES_Dockspace");
    ImGui::DockSpace(dockspace_id, ImGui::GetContentRegionAvail(), ImGuiDockNodeFlags_None);
    ImGui::End();

    ImGui::Begin("Folder", nullptr);
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

    ImGui::Begin("File Explorer", nullptr);

    float itemWidth = 120.0f;  // increased width
    float itemHeight = 140.0f; // increased height to allow more vertical padding
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
                    if (extension == ".obj" || extension == ".gltf" || extension == ".fbx" || extension == ".mtl")
                        icon = ICON_CI_SYMBOL_OBJECT;
                    else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                        icon = ICON_CI_FILE_MEDIA;
                    else if (extension == ".cpp" || extension == ".h" || extension == ".hpp" || extension == ".lua")
                        icon = ICON_CI_FILE_CODE;

                    std::string filename = path.filename().string();
                    std::string shortName = filename;
                    if (shortName.size() > 15)
                        shortName = shortName.substr(0, 10) + "..." + path.extension().string();

                    ImGui::BeginGroup();

                    ImVec2 cellSize(itemWidth, itemHeight);
                    ImVec2 iconSize = ImGui::CalcTextSize(icon);
                    ImVec2 textSize = ImGui::CalcTextSize(shortName.c_str());

                    float verticalOffset = (cellSize.y - (iconSize.y + textSize.y + ImGui::GetStyle().ItemSpacing.y)) * 0.5f;
                    ImGui::Dummy(ImVec2(0.0f, verticalOffset));

                    const std::string pathStr = path.string();

                    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                    {
                        // Lazy-load texture
                        if (imageCache.find(pathStr) == imageCache.end())
                        {
                            unsigned int texID = texLoader.loadTex(pathStr);
                            imageCache[pathStr] = texID;
                        }

                        unsigned int texID = imageCache[pathStr];
                        float imageSize = 48.0f;
                        float imageX = (cellSize.x - imageSize) * 0.5f;
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageX);

                        if (texID != 0)
                            ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2(imageSize, imageSize));
                        else
                            ImGui::TextUnformatted(ICON_CI_FILE_MEDIA);
                    }
                    else
                    {
                        float iconX = (cellSize.x - iconSize.x) * 0.5f;
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconX);
                        ImGui::TextUnformatted(icon);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 4)); // spacing between icon/image and text

                    float textX = (cellSize.x - textSize.x) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textX);
                    ImGui::TextWrapped("%s", shortName.c_str());

                    ImRect bb(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
                    bb.Min.y -= iconSize.y + verticalOffset + ImGui::GetStyle().ItemSpacing.y;
                    bb.Max.y += textSize.y;

                    if (ImGui::IsMouseHoveringRect(bb.Min, bb.Max))
                    {
                        ImGui::BeginTooltip();
                        ImGui::TextUnformatted(pathStr.c_str());
                        currentPath = pathStr;

                        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                        {
                            unsigned int texID = imageCache[pathStr];
                            if (texID != 0)
                            {
                                ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2(100, 100));
                            }
                            else
                            {
                                ImGui::TextDisabled("Failed to load image.");
                            }
                        }

                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            if (extension == ".obj" || extension == ".gltf" || extension == ".fbx" || extension == ".mtl")
                            {
                                std::cout << "Openable\n";
                                std::string path_str = path.string();
                                std::replace(path_str.begin(), path_str.end(), '\\', '/');
                                inputDebug("Info", "loading model" + path_str);
                                renderForFileExplorer.loadModelFirst(path_str);
                            }
                            else if (extension == ".lua")
                            {
                                interface::isCodeEditor = true;
                                loadCode(const_cast<std::string&>(pathStr));
                            }
                            else
                            {
                                inputDebug("Info", "opening file " + path.string());
                            }
                        }

                        ImGui::EndTooltip();

                        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                        {
                            ImGui::OpenPopup("FileContextMenu");
                        }
                    }
                    ImGui::EndGroup();
                    
                }
            }

            if (ImGui::BeginPopup("FileContextMenu"))
            {
                if (ImGui::MenuItem("Rename"))
                {
                    ImGui::OpenPopup("RenameConfirm");
                }
                if (ImGui::MenuItem("Delete"))
                {
                    try
                    {
                        std::filesystem::remove(currentPath);
                        inputDebug("Info", "Succseed to delete file: " + currentPath);
                    }
                    catch (const std::filesystem::filesystem_error &e)
                    {
                        inputDebug("Error", "Failed to delete file: " + std::string(e.what()));
                    }
                }
                ImGui::EndPopup();
            }
            if (ImGui::BeginPopup("RenameConfirm"))
            {
                ImGui::InputText("New Name", &currentPath[0], currentPath.size() + 128); // Provide a buffer for editing
                newName = currentPath;
                if (ImGui::Button("Yes"))
                {
                    std::filesystem::path currentPath = std::filesystem::path(currentDirectory) / newName;
                    if (std::filesystem::exists(currentPath))
                    {
                        inputDebug("Error", "File already exists: " + newName);
                    }
                    else
                    {
                        std::filesystem::rename(currentPath, std::filesystem::path(currentDirectory) / newName);
                        inputDebug("Info", "Succseed to rename file to: " + newName);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("No"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }
        catch (const std::filesystem::filesystem_error &)
        {
        }
        ImGui::EndTable();
    }

    ImGui::End();
}
