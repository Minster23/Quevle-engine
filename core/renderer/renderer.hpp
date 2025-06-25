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
            ROUGHNESS,
            CUBEMAP
        };

        enum class RenderType{
            OBJECT,
            SKYBOX
        };

        void init();
        void shaderLoader(int Index, Renderer::RenderType expression);
        void shaderLoaderLight();
        void LightShaderLink();
        void shaderLink(int Index, Renderer::RenderType expression);
        void drawCallback();
        void drawCleanup();
        void loadTexture(const std::string& texturePath, int Index, Renderer::TextureType expression);
        void loadCubemapTexture(const std::vector<std::string>& faces, int Index);
        void renderObject(const ObjectEntity::ObjectData& objectData, unsigned int shaderProgram);

        void loadModelFirst();
        void LoadAnotherLight();

        static bool Diffuse;
        static bool Specular;
        static bool Normal;
        static bool Metallic;
        static bool Roughness;

        static bool Grid;
    private:
        int success;
        char infoLog[512];
        float intensityForIndex;

        const GLFWvidmode *mode;

        void createGridShaderProgram();
        GLuint compileShader(GLenum type, const char *source);
        void setupGridQuad();

        // Shared resources for light gizmos
        unsigned int lightShaderProgram;
        unsigned int lightVAO;
        unsigned int lightVBO;
    };
}
