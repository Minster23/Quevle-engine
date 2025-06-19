#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <utils/camera/camera.hpp> // Include camera definition
#include <core/renderer/entity/objectEntity.hpp>

namespace QuavleEngine
{
    class Renderer
    {
    public:
        camera cam;

        void init();
        void shaderLoader();
        void shaderLoaderLight();
        void LightShaderLink();
        void shaderLink();
        void drawCallback();
        void drawCleanup();
        unsigned int loadTexture(const std::string& texturePath);
        void renderObject(const ObjectEntity::ObjectData& objectData);

    private:
        int success;
        char infoLog[512];

        const GLFWvidmode* mode;

        ObjectEntity::ObjectData objData;
        ObjectEntity::LightData lightData;
    };
}
