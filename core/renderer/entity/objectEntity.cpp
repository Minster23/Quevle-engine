#include <core/renderer/entity/objectEntity.hpp>

using namespace QuavleEngine;

glm::vec3 ObjectEntity::LightData::lightColor = glm::vec3(1.0f, 9.0f, 1.0f);
glm::vec3 ObjectEntity::LightData::position = glm::vec3(0.0f, 0.0f, -3.0f);
std::vector<ObjectEntity::ObjectData> ObjectEntity::objects;
