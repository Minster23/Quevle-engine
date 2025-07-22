#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/UUID.hpp>

using namespace QuavleEngine;

UUID uids;

std::vector<ObjectEntity::ObjectData> ObjectEntity::objects; // This line is not part of the diff, but it's in the original file
std::vector<ObjectEntity::LightData> ObjectEntity::lights;
std::vector<ObjectEntity::CubeMap> ObjectEntity::CubeMaps;
std::vector<ObjectEntity::Billboard> ObjectEntity::billboards;
std::vector<ObjectEntity::materialData> ObjectEntity::material;

const std::vector<float> ObjectEntity::verticesLight = { // This is a quad for rendering light/camera icons
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

void ObjectEntity::firstLightObject()
{
    LightData light;
    light.UUID = uids.generate_uuid();
    light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    light.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    light.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    light.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light.intensity = 9.0f;
    light.name = "Light object " + std::to_string(lights.size());
    light.isShow = true;
    lights.push_back(light);
}

void ObjectEntity::firstCubemap()
{
    CubeMap cubemap;
    cubemap.cubemapVAO = 0;
    cubemap.cubemapVBO = 0;
    cubemap.fragment = 0;
    cubemap.shaderProgramCubemap = 0;
    cubemap.textureID = 0;
    cubemap.vertex = 0;
    CubeMaps.push_back(cubemap);
}

void ObjectEntity::firstBillboard()
{
    Billboard billboard;
    billboard.UUID = UUID::generate_uuid();
    billboard.name = "Billboard object " + std::to_string(billboards.size());
    billboard.position = glm::vec3(0.0f, 0.0f, 0.0f);

    // Simple quad vertices (centered at origin)
    billboard.vertices = {
        // pos         // uv
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    billboards.push_back(billboard);
}

void ObjectEntity::changeName(int index, std::string newName)
{
    if (index >= 0 && index < objects.size())
    {
        objects[index].name = newName;
    }
}