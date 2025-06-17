#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <utils/camera/camera.hpp> // Include camera definition

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

    private:
        int success;
        char infoLog[512];
        unsigned int vertexShader ,fragmentShader;
        unsigned int vertexShaderLight ,fragmentShaderLight;
        unsigned int shaderProgram, shaderProgramLight;
        unsigned int VBO, VAO;
        unsigned int lightCubeVAO, lightCubeVBO;
        unsigned int texture1;

        const GLFWvidmode* mode;

        std::string readFile(const std::string& path);
    };
}
