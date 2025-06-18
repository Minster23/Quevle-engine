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
        void LightShader();
        void shaderLink();
        void drawCallback();
        void drawCleanup();
        void loadTexture();
        void renderObject(const ObjectEntity::ObjectData& objectData);

    private:
        int success;
        char infoLog[512];
        // unsigned int vertexShader ,fragmentShader;
        unsigned int vertexShaderLight ,fragmentShaderLight;
        unsigned int shaderProgramLight;
        // unsigned int VBO, VAO;
        unsigned int lightCubeVAO, lightCubeVBO;
        // unsigned int texture1;

        const GLFWvidmode* mode;

        std::string readFile(const std::string& path);
        ObjectEntity::ObjectData objData;
    };
}
