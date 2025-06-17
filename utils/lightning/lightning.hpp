#ifndef QUAVLEENGINE_LIGHTNING_HPP
#define QUAVLEENGINE_LIGHTNING_HPP
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace QuavleEngine
{
    class Lightning
    {
    public:
        void init();
        glm::vec3 result;

    private:
        glm::vec3 coral;
        glm::vec3 lightColor;
        glm::vec3 toyColor;
    };
}

#endif // QUAVLEENGINE_LIGHTNING_HPP