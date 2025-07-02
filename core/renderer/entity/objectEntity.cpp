#include <core/renderer/entity/objectEntity.hpp>

using namespace QuavleEngine;

std::vector<ObjectEntity::ObjectData> ObjectEntity::objects;
std::vector<ObjectEntity::LightData> ObjectEntity::lights;
std::vector<ObjectEntity::CubeMap> ObjectEntity::CubeMaps;


void ObjectEntity::firstLightObject()
{
    LightData light;
    light.position = glm::vec3(0.0f, 0.0f, -3.0f);
    light.lightColor = glm::vec3(1.0f, 9.0f, 1.0f);
    light.intensity = 9.0f;
    light.name = "Light object " + std::to_string(lights.size());
    lights.push_back(light);
}

void ObjectEntity::firstCubemap(){
    CubeMap cubemap;
    cubemap.cubemapVAO = 0;
    cubemap.cubemapVBO = 0;
    cubemap.fragment = 0;
    cubemap.shaderProgramCubemap = 0;
    cubemap.textureID = 0;
    cubemap.vertex = 0;
    CubeMaps.push_back(cubemap);
}

void ObjectEntity::changeName(int index, std::string newName)
{
    if (index >= 0 && index < objects.size())
    {
        objects[index].name = newName;
    }
}