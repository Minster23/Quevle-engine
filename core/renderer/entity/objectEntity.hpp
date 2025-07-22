#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace QuavleEngine
{
    class ObjectEntity
    {
    public:

    //! Unused, but kept for future reference
        static const std::vector<float> verticesLight;

        struct materialData
        {
            glm::vec3 ambient = glm::vec3(1.0f);
            glm::vec3 diffuse = glm::vec3(1.0f);
            glm::vec3 specular = glm::vec3(1.0f);
            glm::vec3 emissive = glm::vec3(0.0f);
            float shininess = 1.0f;
            float opacity = 1.0f;
            float shininessStrength = 1.0f;
        };

        struct ObjectData
        {
            std::string locationMode;
            std::string UUID;
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
            unsigned int TexCoords;
            bool hasOwnTexture = false;
            std::string shaderLocation;
            unsigned int shaderProgram;
            unsigned int vertexShader;
            unsigned int fragmentShader;
            float *vertices;
            unsigned int *indices;
            size_t indicesCount;
            size_t floatsPerVertex;
            std::string name;
            materialData material;
            unsigned int diffuseTextureID;
            unsigned int specularTextureID;
            unsigned int normalTextureID;    // Normal map
            unsigned int metallicTextureID;  // Metallic map
            unsigned int roughnessTextureID; // Roughness map
            unsigned int HeightTextureID;
            unsigned int ambientTextureID;      // Ambient map
            unsigned int emissiveTextureID;     // Emissive map
            unsigned int opacityTextureID;      // Opacity / transparency map
            unsigned int displacementTextureID; // Displacement map
            std::vector<std::string> scriptLocation;
            std::vector<int> components;
            size_t vertexCount;                 // Added for vertex count
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            bool isShow = true;
            bool isSelected = false;
        };

        enum LightType
        {
            Point,
            Directional
        };

        struct LightData
        {
            std::string UUID;
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            glm::vec3 lightColor;
            unsigned int textureID;
            std::string name;
            float intensity;
            bool isShow = true;
            bool isSelected = false;
            std::vector<int> components;
            LightType type = Point;
            float range = 1.0f; // For point lights
        };

        struct CubeMap
        {
            unsigned int cubemapVAO, cubemapVBO;
            unsigned int shaderProgramCubemap;
            unsigned int vertex, fragment;
            unsigned int textureID;
        };

        struct Billboard
        {
            std::string UUID;
            std::string name;
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
            size_t indicesCount = 0;
            bool lookAt = true;
            bool isSelected;
            std::string texLocation;
            glm::mat4 model = glm::mat4(1.0f);
            unsigned int vertexShader = 0;
            unsigned int fragmentShader = 0;
            unsigned int shaderProgram = 0;
            unsigned int VAO = 0;
            unsigned int VBO = 0;
            unsigned int textureID;

            glm::vec3 position = glm::vec3(0.0f);
            //* DUMMY 
            glm::vec3 rotation = glm::vec3(0.0f);
            glm::vec3 scale = glm::vec3(0.0f);

            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            bool isShow = true;
            
        };

        static std::vector<materialData> material;
        static std::vector<LightData> lights;
        static std::vector<ObjectData> objects;
        static std::vector<CubeMap> CubeMaps;
        static std::vector<Billboard> billboards;

        void changeName(int index, std::string newName);
        void firstLightObject();
        void firstCubemap();
        void firstBillboard();

    private:
    };
}