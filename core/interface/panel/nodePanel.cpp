#define IMGUI_DEFINE_MATH_OPERATOR
#include <core/interface/interface.hpp>
#include <core/interface/nodes.h>
#include <core/renderer/entity/objectEntity.hpp>
#include <utils/Debug.h>
#include <core/renderer/renderer.hpp>

using namespace QuavleEngine;
ObjectEntity enityForNodeMaterial;
interface interfaceForNode;
Renderer renderForNodePanel;

// Definitions for global variables from nodes.h
ImFlow::ImNodeFlow mINF;
nlohmann::json j;
std::vector<std::string> nodesList = {"VariableNode", "ColorNode", "Multiply Float", "Subtract Vec3"};
int indexNodeSelector = -1;
std::string shaderPath = config["LOCATION"]["shader"];
bool nodePanelOpen = false;

void interface::nodePanel(int index)
{
    if (nodePanelOpen)
    {
        if (index < 0 || static_cast<size_t>(index) >= enityForNodeMaterial.objects.size())
        {
            closeEditor();
            return;
        }

        static NodeEditor *neditor = nullptr;

        ImGui::Begin(("Material " + enityForNodeMaterial.objects[index].name).c_str(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar);

        if (ImGui::Button("Compile")) // This button is nested inside "Save" menu item, which is unusual.
        {
            DEBUG_PRINT("Saving material cliked");
            OutputNode *output = neditor->getOutputNode();

            exportMaterialToShader(
                output->baseColor,
                output->opacity,
                output->metallic,
                output->roughness,
                (enityForNodeMaterial.objects[index].name + ".shd").c_str());

            enityForNodeMaterial.objects[index].shaderLocation = (shaderPath + enityForNodeMaterial.objects[index].name + ".shd"); // This should be .frag or .vert

            DEBUG_PRINT("Shader location: " + enityForNodeMaterial.objects[index].shaderLocation);
            DEBUG_PRINT("Saving material shader");

            renderForNodePanel.shaderLoader(InspectorIndex, Renderer::RenderType::OBJECT);
            renderForNodePanel.shaderLink(InspectorIndex, Renderer::RenderType::OBJECT);
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            closeEditor();
        }
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            save(shaderPath + enityForNodeMaterial.objects[index].name + ".json");
        }
        ImGui::SameLine();
        if (ImGui::Button("load"))
        {
            load(shaderPath + enityForNodeMaterial.objects[index].name + ".json");
        }

        // Example values for exportMaterialToShader, replace with actual node outputs
        glm::vec3 baseColor = glm::vec3(1.0f, 0.5f, 0.2f); // Example base color

        ImVec2 editor_size = ImGui::GetContentRegionAvail();
        if (!neditor)
        {
            neditor = new NodeEditor((int)editor_size.x, (int)editor_size.y);
        }

        neditor->set_size(editor_size);
        neditor->draw();

        ImGui::End();
    }
}

void exportMaterialToShader(const glm::vec3 &baseColor, float opacity, float metallic, float roughness, const std::string &filepath)
{
    std::ofstream file(shaderPath + filepath);
    if (!file.is_open())
        return;

    file << "#version 330 core\n\n";

    // Outputs
    file << "out vec4 FragColor;\n\n";

    // Inputs
    file << "in vec3 Normal;\n";
    file << "in vec3 FragPos;\n";
    file << "in vec2 TexCoords;\n";
    file << "in vec4 FragPosLightSpace;\n"; // Added from second shader

    // Uniforms
    file << "uniform vec3 viewPos;\n\n";

    // Material properties
    file << "// Node-based material values\n";
    file << "vec3 baseColor = vec3("
         << baseColor.r << ", "
         << baseColor.g << ", "
         << baseColor.b << ");\n";
    file << "float opacity = " << opacity << ";\n";
    file << "float metallic = " << metallic << ";\n";
    file << "float roughness = " << roughness << ";\n\n";

    // Shader main function
    file << "// Minimal PBR output\n";
    file << "void main()\n";
    file << "{\n";
    file << "    vec3 color = baseColor;\n";
    file << "    color = pow(color, vec3(1.0 / 2.2)); // gamma correction\n";
    file << "    FragColor = vec4(color, opacity);\n";
    file << "}\n";

    file.close();
}

void save(const std::string &filename)
{
    nlohmann::json jnodes = nlohmann::json::array();
    nlohmann::json jlinks = nlohmann::json::array();

    for (const auto &[uid, node] : mINF.getNodes())
    {
        if (!node)
            continue;

        nlohmann::json jnode;
        jnode["id"] = uid;
        jnode["title"] = node->getName();
        jnode["pos"] = {node->getPos().x, node->getPos().y};

        const std::type_info &type = typeid(*node);

        if (type == typeid(ColorNode))
        {
            jnode["type"] = "ColorNode";

            // Try to get the value from the output pin named "RGB"
            glm::vec3 valNode(0.0f);

            for (const auto &pin : node->getOuts())
            {
                if (pin && pin->getName() == "RGB")
                {
                    auto outPin = std::dynamic_pointer_cast<ImFlow::OutPin<glm::vec3>>(pin);
                    if (outPin)
                    {
                        valNode = outPin->val(); // ✅ pulls from behaviour()
                        break;
                    }
                }
            }

            jnode["value"]["valueNode"] = {valNode.r, valNode.g, valNode.b};
        }

        else if (type == typeid(VariableNode))
        {
            jnode["type"] = "VariableNode";

            float valNode = 0;

            for (const auto &pin : node->getOuts())
            {
                if (pin->getName() == "Out")
                {
                    auto outPin = std::dynamic_pointer_cast<ImFlow::OutPin<float>>(pin);
                    if (outPin)
                    {
                        valNode = outPin->val(); // ✅ Safe
                    }
                }
            }
            jnode["value"]["valueNode"] = valNode;
        }
        else if (type == typeid(MultiplyFloat))
        {
            jnode["type"] = "MultiplyFloat";
        }
        else if (type == typeid(SubtractVec3))
        {
            jnode["type"] = "SubtractVec3";
        }
        else if (type == typeid(OutputNode))
        {
            jnode["type"] = "OutputNode";
        }
        else
        {
            jnode["type"] = "Unknown";
        }

        jnodes.push_back(jnode);
    }

    for (const auto &linkWeak : mINF.getLinks())
    {
        if (auto link = linkWeak.lock())
        {
            auto fromPin = link->left();
            auto toPin = link->right();

            if (!fromPin || !toPin)
                continue;

            auto fromNode = fromPin->getParent();
            auto toNode = toPin->getParent();

            if (!fromNode || !toNode)
                continue;

            jlinks.push_back({{"from_node", fromNode->getUID()},
                              {"from_pin", fromPin->getName()},
                              {"to_node", toNode->getUID()},
                              {"to_pin", toPin->getName()}});
        }
    }

    nlohmann::json j;
    j["nodes"] = jnodes;
    j["links"] = jlinks;

    std::ofstream out(filename);
    if (!out.is_open())
    {
        interfaceForNode.inputDebug("Warning", "Cannot save shader, failed to open file");
        return;
    }
    out << j.dump(4);
}

void load(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        interfaceForNode.inputDebug("Warning", "Cannot load shader, File not found");
        return;
    }

    mINF.getNodes().clear();
    nlohmann::json j;
    in >> j;

    // This map holds: JSON id → pointer to the created node
    std::unordered_map<uint64_t, std::shared_ptr<ImFlow::BaseNode>> idToNode;

    // Create nodes
    for (const auto &node : j["nodes"])
    {
        uint64_t id = node["id"];
        ImVec2 pos = {node["pos"][0], node["pos"][1]};
        std::string type = node["type"];
        std::string title = node["title"];

        std::shared_ptr<ImFlow::BaseNode> createdNode = nullptr;

        if (type == "ColorNode")
        {
            auto varNode = mINF.placeNodeAt<ColorNode>(pos);

            if (node["value"].contains("valueNode") && node["value"]["valueNode"].is_array())
            {
                const auto &arr = node["value"]["valueNode"];
                if (arr.size() == 3)
                {
                    glm::vec3 val(arr[0], arr[1], arr[2]);
                    varNode->setValue(val);
                }
            }

            createdNode = varNode;
        }
        else if (type == "VariableNode")
        {
            // Place node
            auto varNode = mINF.placeNodeAt<VariableNode>(pos);

            if (node["value"].contains("valueNode"))
            {
                float val = node["value"]["valueNode"];
                varNode->setValue(val);
            }

            createdNode = varNode;
        }
        else if (type == "MultiplyFloat")
        {
            createdNode = mINF.placeNodeAt<MultiplyFloat>(pos);
        }
        else if (type == "SubtractVec3")
        {
            createdNode = mINF.placeNodeAt<SubtractVec3>(pos);
        }
        else if (type == "OutputNode")
        {
            createdNode = mINF.placeNodeAt<OutputNode>(pos);
        }

        if (createdNode)
        {
            createdNode->setTitle(title);
            idToNode[id] = createdNode;
        }
    }

    // Create links
    for (const auto &link : j["links"])
    {
        uint64_t fromID = link["from_node"];
        uint64_t toID = link["to_node"];
        std::string fromPinName = link["from_pin"];
        std::string toPinName = link["to_pin"];

        auto fromIt = idToNode.find(fromID);
        auto toIt = idToNode.find(toID);

        if (fromIt == idToNode.end() || toIt == idToNode.end())
        {
            interfaceForNode.inputDebug("Warning", "Invalid node ID in link");
            continue;
        }

        auto fromNode = fromIt->second;
        auto toNode = toIt->second;

        std::shared_ptr<ImFlow::Pin> outPin = nullptr;
        std::shared_ptr<ImFlow::Pin> inPin = nullptr;

        for (auto &pin : fromNode->getOuts())
            if (pin->getName() == fromPinName)
                outPin = pin;

        for (auto &pin : toNode->getIns())
            if (pin->getName() == toPinName)
                inPin = pin;

        if (outPin && inPin)
        {
            outPin->createLink(inPin.get());
        }
        else
        {
            interfaceForNode.inputDebug("Warning", "Pin not found when linking");
        }
    }
}

void openEditor(const std::string &filename)
{
    mINF.getNodes().clear();
    std::ifstream path(filename);
    if (!path.is_open())
    {
        starterEditorOpen();
        nodePanelOpen = true;
        interfaceForNode.inputDebug("Info", "Make new file");
    }
    else
    {
        nodePanelOpen = true;
        load(filename);
    }
}

void closeEditor()
{
    nodePanelOpen = false;
}

void starterEditorOpen()
{
    auto colorNode = mINF.addNode<ColorNode>({0, 0});
    auto outputNode = mINF.addNode<OutputNode>({200, 0});

    // Connect them
    connectColorToOutput(colorNode, outputNode);
}