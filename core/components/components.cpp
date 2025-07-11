#include <core/components/components.hpp>

using namespace QuavleEngine;


void ComponentsManager::load(){
    components[0] = new lightSetting();
    components[1] = new lightSettting();
}