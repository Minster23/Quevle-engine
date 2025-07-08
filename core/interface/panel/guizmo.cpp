#include <core/interface/interface.hpp>
#include <ImGuizmo.h>
#include <core/renderer/entity/objectEntity.hpp>

using namespace QuavleEngine;
ObjectEntity entittyGuizmo;

void interface::guizmoSetting(glm::mat4 &modelMatrix, const glm::mat4 &view, const glm::mat4 &projection, ImVec2 pos, ImVec2 size)
{
    // 1. Build modelMatrix from object BEFORE rendering gizmo
    const auto& obj = entittyGuizmo.objects[InspectorIndex];

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
        ImGuizmo::TRANSLATE,
        ImGuizmo::WORLD,
        glm::value_ptr(modelMatrix)
    );

    // 4. Decompose matrix back into object transform
    float translation[3], rotation[3], scale[3];
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

    entittyGuizmo.objects[InspectorIndex].position = glm::vec3(translation[0], translation[1], translation[2]);
    entittyGuizmo.objects[InspectorIndex].rotation = glm::vec3(rotation[0], rotation[1], rotation[2]); // still in degrees
    entittyGuizmo.objects[InspectorIndex].scale    = glm::vec3(scale[0], scale[1], scale[2]);
}
