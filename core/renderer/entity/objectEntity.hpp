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
        };

        static std::vector<ObjectData> objects; // Public vector of ObjectData

        struct LightData
        {
            unsigned int lightCubeVAO;
            unsigned int lightCubeVBO;
            static glm::vec3 position;
            static glm::vec3 lightColor;
            unsigned int vertexShaderLight, fragmentShaderLight;
            unsigned int shaderProgramLight;
        };

        static std::vector<LightData> lights;

        void firstLightObject();

    private:
    };
}