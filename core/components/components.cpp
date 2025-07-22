#include <core/components/components.hpp>

using namespace QuavleEngine;

void ComponentsManager::load(){
    components[0] = new light_setting();
    components[1] = new create_script();
}