#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/model/model.hpp>

namespace QuavleEngine
{
    extern interface intfc;

    class Renderer
    {
    public:
        enum class TextureType
        {
            DIFFUSE,
            SPECULAR,
            NORMAL,
            METALLIC,
            ROUGHNESS,
            HEIGHT,
            AMBIENT,
            EMISSIVE,
            OPACITY,
            DISPLACEMENT,
            CUBEMAP
        };

        enum class RenderType{
            OBJECT,
            SKYBOX,
            BILLBOARD
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
        void SetupBillboards(int index);
        void SetUpImageBillboard(int index, const std::string &image_path);
        glm::mat4 getBillboardModel(const glm::vec3& pos, const glm::mat4& view);

        void loadModelFirst(std::string path);
        void LoadAnotherLight();

        static bool Diffuse;
        static bool Specular;
        static bool Normal;
        static bool Metallic;
        static bool Roughness;

        static bool Grid;
        static glm::vec3 gridColor;
        static float gridSpacing;

        static glm::mat4 projection;
        static glm::mat4 view;
        static glm::mat4 model;

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
