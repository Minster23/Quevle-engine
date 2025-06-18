#include <string>
#include <iostream>
#include <glm/glm.hpp>


namespace QuavleEngine{
    class ObjectEntity {
    public:
    struct materialData {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    struct ObjectData{
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int textureID;
        unsigned int shaderProgram;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        std::string name;
        materialData material;
    };
    private:

};
}