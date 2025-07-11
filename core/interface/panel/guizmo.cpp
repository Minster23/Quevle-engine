#include <core/interface/interface.hpp>
#include <ImGuizmo.h>
#include <core/renderer/entity/objectEntity.hpp>
#include <algorithm>

using namespace QuavleEngine;
ObjectEntity entittyGuizmo;

void interface::guizmoSetting(glm::mat4 &modelMatrix, const glm::mat4 &view, const glm::mat4 &projection, ImVec2 pos, ImVec2 size, GUIZMOTARGET target)
{
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(ImGuiKey_Z))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    if (target == GUIZMOTARGET::OBJECT && InspectorIndex != -1)
    {
        // 1. Build modelMatrix from object BEFORE rendering gizmo
        const auto &obj = entittyGuizmo.objects[InspectorIndex];

        modelMatrix = glm::translate(modelMatrix, obj.position);
        modelMatrix = glm::mat4(1.0f);

        // Apply translation first (this sets gizmo position)
        modelMatrix = glm::translate(modelMatrix, obj.position);

        // Apply rotation in radians
        glm::vec3 rotRad = glm::radians(obj.rotation);
        modelMatrix = glm::rotate(modelMatrix, rotRad.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.z, glm::vec3(0, 0, 1));

        // Apply scale last
        modelMatrix = glm::scale(modelMatrix, obj.scale);

        // 2. Setup ImGuizmo viewport area
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        // 3. Manipulate
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            mCurrentGizmoOperation,
            ImGuizmo::WORLD,
            glm::value_ptr(modelMatrix));

        // 4. Decompose matrix back into object transform
        float translation[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

        entittyGuizmo.objects[InspectorIndex].position = glm::vec3(translation[0], translation[1], translation[2]);
        entittyGuizmo.objects[InspectorIndex].rotation = glm::vec3(rotation[0], rotation[1], rotation[2]); // still in degrees
        entittyGuizmo.objects[InspectorIndex].scale = glm::vec3(scale[0], scale[1], scale[2]);
    }

    //* Multi Select
    if (target == GUIZMOTARGET::OBJECT && !selectedNames.empty())
    {
        // Calculate the centroid of selected objects for the gizmo position
        glm::vec3 centroidPosition(0.0f);
        glm::vec3 centroidRotation(0.0f);
        glm::vec3 centroidScale(1.0f); // Default scale
        int selectedCount = 0;

        // Store initial transforms for delta calculations
        std::vector<glm::vec3> initialPositions;
        std::vector<glm::vec3> initialRotations;
        std::vector<glm::vec3> initialScales;

        for (const auto &name : selectedNames)
        {
            auto it = std::find_if(entittyGuizmo.objects.begin(), entittyGuizmo.objects.end(),
                                   [&](const auto &obj)
                                   { return obj.name == name && obj.isSelected; });
            if (it != entittyGuizmo.objects.end())
            {
                centroidPosition += it->position;
                centroidRotation += it->rotation;
                centroidScale += it->scale;
                selectedCount++;

                // Store initial transforms
                initialPositions.push_back(it->position);
                initialRotations.push_back(it->rotation);
                initialScales.push_back(it->scale);
            }
        }

        if (selectedCount > 0)
        {
            centroidPosition /= static_cast<float>(selectedCount);
            centroidRotation /= static_cast<float>(selectedCount);
            centroidScale /= static_cast<float>(selectedCount);
        }

        // Initialize modelMatrix with the centroid transforms
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, centroidPosition);

        // Apply rotation in radians
        glm::vec3 rotRad = glm::radians(centroidRotation);
        modelMatrix = glm::rotate(modelMatrix, rotRad.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.z, glm::vec3(0, 0, 1));

        // Apply scale
        modelMatrix = glm::scale(modelMatrix, centroidScale);

        // Setup ImGuizmo
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        // Manipulate the gizmo
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            mCurrentGizmoOperation,
            mCurrentGizmoMode,
            glm::value_ptr(modelMatrix));

        // Decompose matrix back into transform components
        float translation[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

        // Apply transforms to all selected objects
        if (ImGuizmo::IsUsing())
        {
            glm::vec3 newPosition = glm::vec3(translation[0], translation[1], translation[2]);
            glm::vec3 newRotation = glm::vec3(rotation[0], rotation[1], rotation[2]);
            glm::vec3 newScale = glm::vec3(scale[0], scale[1], scale[2]);

            // Calculate deltas from centroid
            glm::vec3 positionDelta = newPosition - centroidPosition;
            glm::vec3 rotationDelta = newRotation - centroidRotation;
            glm::vec3 scaleDelta = newScale / centroidScale;

            // Apply to all selected objects
            size_t objIndex = 0;
            for (auto &obj : entittyGuizmo.objects)
            {
                if (obj.isSelected && objIndex < initialPositions.size())
                {
                    if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
                    {
                        obj.position = initialPositions[objIndex] + positionDelta;
                    }
                    else if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
                    {
                        // For rotation, we apply the delta rotation to each object's original rotation
                        obj.rotation = initialRotations[objIndex] + rotationDelta;
                    }
                    else if (mCurrentGizmoOperation == ImGuizmo::SCALE)
                    {
                        // For scale, we multiply by the scale delta
                        obj.scale = initialScales[objIndex] * scaleDelta;
                    }
                    objIndex++;
                }
            }
        }
    }

    if (target == GUIZMOTARGET::LIGHT && InspectorIndexUtility != -1)
    {
        // 1. Build modelMatrix from object BEFORE rendering gizmo
        const auto &obj = entittyGuizmo.lights[InspectorIndexUtility];

        modelMatrix = glm::mat4(1.0f);

        // Apply translation first (this sets gizmo position)
        modelMatrix = glm::translate(modelMatrix, obj.position);

        // Apply rotation in radians
        glm::vec3 rotRad = glm::radians(obj.rotation);
        modelMatrix = glm::rotate(modelMatrix, rotRad.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.z, glm::vec3(0, 0, 1));

        // Apply scale last
        modelMatrix = glm::scale(modelMatrix, obj.scale);

        // 2. Setup ImGuizmo viewport area
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        // 3. Manipulate
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            mCurrentGizmoOperation,
            ImGuizmo::WORLD,
            glm::value_ptr(modelMatrix));

        // 4. Decompose matrix back into object transform
        float translation[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

        entittyGuizmo.lights[InspectorIndexUtility].position = glm::vec3(translation[0], translation[1], translation[2]);
        entittyGuizmo.lights[InspectorIndexUtility].rotation = glm::vec3(rotation[0], rotation[1], rotation[2]); // still in degrees
        entittyGuizmo.lights[InspectorIndexUtility].scale = glm::vec3(scale[0], scale[1], scale[2]);
    }

    if (target == GUIZMOTARGET::CAMERA && InspectorIndexUtility != -1)
    {
        // 1. Build modelMatrix from object BEFORE rendering gizmo
        const auto &obj = cameras[InspectorIndexUtility];

        modelMatrix = glm::mat4(1.0f);

        // Apply translation first (this sets gizmo position)
        modelMatrix = glm::translate(modelMatrix, obj.cameraPos);

        // Apply rotation in radians
        glm::vec3 rotRad = glm::radians(obj.rotation);
        modelMatrix = glm::rotate(modelMatrix, rotRad.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.z, glm::vec3(0, 0, 1));

        // Apply scale last
        modelMatrix = glm::scale(modelMatrix, obj.scale);

        // 2. Setup ImGuizmo viewport area
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        // 3. Manipulate
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            mCurrentGizmoOperation,
            ImGuizmo::WORLD,
            glm::value_ptr(modelMatrix));

        // 4. Decompose matrix back into object transform
        float translation[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

        cameras[InspectorIndexUtility].cameraPos = glm::vec3(translation[0], translation[1], translation[2]);
        cameras[InspectorIndexUtility].rotation = glm::vec3(rotation[0], rotation[1], rotation[2]); // still in degrees
        cameras[InspectorIndexUtility].scale = glm::vec3(scale[0], scale[1], scale[2]);
    }

    if (target == GUIZMOTARGET::BILLBOARD && InspectorIndexUtility != -1)
    {
        // 1. Build modelMatrix from object BEFORE rendering gizmo
        const auto &obj = entittyGuizmo.billboards[InspectorIndexUtility];

        modelMatrix = glm::mat4(1.0f);

        // Apply translation first (this sets gizmo position)
        modelMatrix = glm::translate(modelMatrix, obj.position);

        // Apply rotation in radians
        glm::vec3 rotRad = glm::radians(obj.rotation);
        modelMatrix = glm::rotate(modelMatrix, rotRad.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotRad.z, glm::vec3(0, 0, 1));

        // Apply scale last
        modelMatrix = glm::scale(modelMatrix, obj.scale);

        // 2. Setup ImGuizmo viewport area
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        // 3. Manipulate
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            mCurrentGizmoOperation,
            ImGuizmo::WORLD,
            glm::value_ptr(modelMatrix));

        // 4. Decompose matrix back into object transform
        float translation[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

        entittyGuizmo.billboards[InspectorIndexUtility].position = glm::vec3(translation[0], translation[1], translation[2]);
        entittyGuizmo.billboards[InspectorIndexUtility].rotation = glm::vec3(rotation[0], rotation[1], rotation[2]); // still in degrees
        entittyGuizmo.billboards[InspectorIndexUtility].scale = glm::vec3(scale[0], scale[1], scale[2]);
    }
}
