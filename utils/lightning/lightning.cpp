#include <utils/lightning/lightning.hpp>

using namespace QuavleEngine;
void Lightning::init() {
    coral = glm::vec3(1.0f, 0.5f, 0.31f); 
    lightColor = glm::vec3(0.33f, 0.42f, 0.18f);
    toyColor = glm::vec3(1.0f, 0.5f, 0.31f);
    result =  lightColor * toyColor; // Example operation
}


