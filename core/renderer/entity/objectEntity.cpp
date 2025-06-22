#include <core/renderer/entity/objectEntity.hpp>

using namespace QuavleEngine;

glm::vec3 ObjectEntity::LightData::lightColor = glm::vec3(1.0f, 9.0f, 1.0f);
glm::vec3 ObjectEntity::LightData::position = glm::vec3(0.0f, 0.0f, -3.0f);
std::vector<ObjectEntity::ObjectData> ObjectEntity::objects;
std::vector<ObjectEntity::LightData> ObjectEntity::lights;
std::vector<ObjectEntity::CubeMap> ObjectEntity::CubeMaps;


void ObjectEntity::firstLightObject()
{
    LightData light;
    light.lightCubeVAO = 0;
    light.lightCubeVBO = 0;
    light.position = glm::vec3(0.0f, 0.0f, -3.0f);
    light.lightColor = glm::vec3(1.0f, 9.0f, 1.0f);
    light.vertexShaderLight = 0;
    light.fragmentShaderLight = 0;
    light.shaderProgramLight = 0;

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