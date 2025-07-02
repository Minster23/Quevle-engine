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
        struct materialData
        {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
        };

        struct ObjectData
        {
            std::string UUID;
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
            unsigned int TexCoords;
            unsigned int shaderProgram;
            unsigned int vertexShader;
            unsigned int fragmentShader;
            float* vertices;
            unsigned int* indices; 
            size_t indicesCount;
            size_t floatsPerVertex; 
            std::string name;
            materialData material;
            unsigned int diffuseTextureID; 
            unsigned int specularTextureID;
            unsigned int normalTextureID;    // Normal map
            unsigned int metallicTextureID;  // Metallic map
            unsigned int roughnessTextureID; // Roughness map
            size_t vertexCount; // Added for vertex count
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            bool isShow = true;
            bool isSelected = false;
        };

        struct LightData
        {
            glm::vec3 position;
            glm::vec3 lightColor;
            std::string name;
            float intensity;
        };

        struct CubeMap
        {
            unsigned int cubemapVAO, cubemapVBO;
            unsigned int shaderProgramCubemap;
            unsigned int vertex, fragment;
            unsigned int textureID;
        };

        static std::vector<LightData> lights;
        static std::vector<ObjectData> objects;
        static std::vector<CubeMap> CubeMaps;

        void changeName(int index, std::string newName);
        void firstLightObject();
        void firstCubemap();

    private:
    };
}