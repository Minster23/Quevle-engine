#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <utils/camera/camera.hpp> // Include camera definition
#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/model.hpp>

namespace QuavleEngine
{
    class Renderer
    {
    public:
        camera cam;

        enum class TextureType
        {
            DIFFUSE,
            SPECULAR,
            NORMAL,
            METALLIC,
            ROUGHNESS
        };

        void init();
        void shaderLoader(int Index);
        void shaderLoaderLight();
        void LightShaderLink();
        void shaderLink(int Index);
        void drawCallback();
        void drawCleanup();
        void loadTexture(const std::string& texturePath, int Index, Renderer::TextureType expression);
        void renderObject(const ObjectEntity::ObjectData& objectData, unsigned int shaderProgram);

    private:
        int success;
        char infoLog[512];

        const GLFWvidmode* mode;
        ObjectEntity objectEntity;
        ObjectEntity::LightData lightData;
    };
}
