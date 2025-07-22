#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>

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
            CUBEMAP,
            BILLBOARD,
            CAMERA
        };

        enum class RenderType{
            OBJECT,
            SKYBOX,
            BILLBOARD,
            CAMERA
        };

        static float u_exposure;
        static float u_gamma;
        static float u_iblStrength;
        static glm::vec3 u_iblColor;
        static glm::vec3 u_groundColor;

        // Post-Processing Control Variables
        static bool u_enableSSAO;
        static bool u_enableSSR;
        static bool u_enableBloom;
        static bool u_enableToneMapping;
        static bool u_enableSharpening;

        // SSAO (Screen Space Ambient Occlusion) Parameters
        static float u_ssaoRadius;
        static float u_ssaoIntensity;
        static int u_ssaoSamples;
        static float u_ssaoBias;

        // SSR (Screen Space Reflection) Parameters
        static float u_ssrMaxDistance;
        static float u_ssrStepSize;
        static int u_ssrMaxSteps;
        static float u_ssrThickness;
        static float u_ssrFalloff;

        // Bloom Parameters
        static float u_bloomThreshold;
        static float u_bloomIntensity;
        static float u_bloomRadius;

        // Tone Mapping Parameters
        static int u_toneMapMode; // 0=ACES, 1=Reinhard, 2=Filmic, 3=Uncharted2
        static float u_toneMapWhitePoint;

        // Sharpening Parameters
        static float u_sharpenStrength;
        static float u_sharpenRadius;

        // Screen resolution for post-processing calculations
        static float u_screenResolution[2];

        void init();
        void shaderLoader(int Index, Renderer::RenderType expression);
        void shaderLoaderLight();
        void LightShaderLink();
        void loadBillboard();
        void loadAnotherCam();
        void shaderLink(int Index, Renderer::RenderType expression);
        void drawCallback();
        void drawCleanup();
        void loadTexture(const std::string& texturePath, int Index, Renderer::TextureType expression);
        void loadCubemapTexture(const std::vector<std::string>& faces, int Index);
        void renderObject(const ObjectEntity::ObjectData& objectData, unsigned int shaderProgram);
        void SetupBillboards(int index);
        void SetUpImageBillboard(int index, const std::string &image_path);
        glm::mat4 getBillboardModel(const glm::vec3& pos, const glm::mat4& view);

        bool loadModelFirst(std::string path);
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

        static glm::mat4 modelLight;

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
