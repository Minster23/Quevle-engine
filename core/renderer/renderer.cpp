// --- Includes ---
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <stb_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/renderer/renderer.hpp>
#include <core/renderer/shader_h.h>
#include <utils/Debug.h>

// --- Constants ---
namespace
{
    constexpr char VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/vertex.glsl";
    constexpr char FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/fragment.glsl";
    constexpr char LIGHT_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightVert.glsl";
    constexpr char LIGHT_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightFrag.glsl";
    constexpr char DIFFUSE_TEXTURE_PATH[] = "D:/ReaperMedia/Compositing/Media/CustomUVChecker_byValle_1K.png";
    constexpr char SPECULAR_TEXTURE_PATH[] = "C:/Users/athilah/Downloads/container2_specular.png";
}

// --- Cube and Light Data ---
float verticesLight[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -10.0f),
    glm::vec3(0.0f, 0.0f, -20.0f),
    glm::vec3(0.0f, 0.0f, -30.0f)
};
// Cubes in a line, spaced 10 units apart

glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 0.0f, -10.0f),
    glm::vec3(2.0f, 0.0f, -20.0f),
    glm::vec3(2.0f, 0.0f, -30.0f)
};
// Lights facing cubes, each 10 units away on the +X axis

using namespace QuavleEngine;

WindowManager windowManager;
// --- Renderer Implementation ---

void Renderer::init()
{
    DEBUG_PRINT("Renderer::init() called");
    Model model("D:/ReaperMedia/Compositing/Media/genshin_impact_-_furina/scene.gltf", false);
    // Copy static objects to local objectEntity.objects for this renderer
    objectEntity.objects = ObjectEntity::objects;
    shaderLoaderLight();
    LightShaderLink();
    cam.init();
    for (size_t i = 0; i < objectEntity.objects.size(); ++i) {
        shaderLoader(i);
        shaderLink(i);
        // Load textures for each object (if you have per-object textures, set the path accordingly)
        loadTexture(DIFFUSE_TEXTURE_PATH, i);
        // objectEntity.objects[i].specularTextureID = loadTexture(SPECULAR_TEXTURE_PATH);
    }
    if (objectEntity.objects.size() == 0)
    {
        DEBUG_PRINT("No objects to render. Exiting drawCallback.");
    }
    for (int j = 0; j < objectEntity.objects.size(); ++j)
    {
        DEBUG_PRINT("IN RENDERER Object " + std::to_string(j) + ": " + objectEntity.objects[j].name +
                    ", Diffuse Texture ID: " + std::to_string(objectEntity.objects[j].diffuseTextureID) +
                    ", Specular Texture ID: " + std::to_string(objectEntity.objects[j].specularTextureID) +
                    ", Shader Program: " + std::to_string(objectEntity.objects[j].shaderProgram) +
                    ", vertices: " + std::to_string(reinterpret_cast<uintptr_t>(objectEntity.objects[j].vertices))
                );
    }
}

void Renderer::shaderLoader(int Index)
{
    //* Load and compile vertex and fragment shaders for the object
    DEBUG_PRINT("Renderer::shaderLoader() called");
    std::string vertSource = readFile(VERT_SHADER_PATH);
    std::string fragSource = readFile(FRAG_SHADER_PATH);
    const char *vertexShaderSource = vertSource.c_str();
    const char *fragmentShaderSource = fragSource.c_str();

    // Vertex Shader
    objectEntity.objects[Index].vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(objectEntity.objects[Index].vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(objectEntity.objects[Index].vertexShader);
    glGetShaderiv(objectEntity.objects[Index].vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objectEntity.objects[Index].vertexShader, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    // Fragment Shader
    objectEntity.objects[Index].fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(objectEntity.objects[Index].fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(objectEntity.objects[Index].fragmentShader);
    glGetShaderiv(objectEntity.objects[Index].fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objectEntity.objects[Index].fragmentShader, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    // Shader Program
    objectEntity.objects[Index].shaderProgram = glCreateProgram();
    glAttachShader(objectEntity.objects[Index].shaderProgram, objectEntity.objects[Index].vertexShader);
    glAttachShader(objectEntity.objects[Index].shaderProgram, objectEntity.objects[Index].fragmentShader);
    glLinkProgram(objectEntity.objects[Index].shaderProgram);
    glGetProgramiv(objectEntity.objects[Index].shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(objectEntity.objects[Index].shaderProgram, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::SHADER::LINKING_FAILED\n" + std::string(infoLog));
    }

    //* Clean up shaders after linking
    glDeleteShader(objectEntity.objects[Index].vertexShader);
    glDeleteShader(objectEntity.objects[Index].fragmentShader);
}

void Renderer::shaderLink(int Index)
{
    //* Generate and bind VAO, VBO, and EBO for the object
    glGenVertexArrays(1, &objectEntity.objects[Index].VAO);
    glGenBuffers(1, &objectEntity.objects[Index].VBO);
    glGenBuffers(1, &objectEntity.objects[Index].EBO);
    glBindVertexArray(objectEntity.objects[Index].VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.objects[Index].VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        objectEntity.objects[Index].vertexCount * objectEntity.objects[Index].floatsPerVertex * sizeof(float),
        objectEntity.objects[Index].vertices,
        GL_STATIC_DRAW
    );

    // Index buffer
    if (objectEntity.objects[Index].indices && objectEntity.objects[Index].indicesCount > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEntity.objects[Index].EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            objectEntity.objects[Index].indicesCount * sizeof(unsigned int),
            objectEntity.objects[Index].indices,
            GL_STATIC_DRAW
        );
    }

    //* Set up vertex attributes: position, normal, tangent/bitangent, texcoord, color
    size_t offset = 0;
    int attribIndex = 0;
    // Position (always present)
    glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].floatsPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(attribIndex++);
    offset += 3;
    // Normal
    if (objectEntity.objects[Index].floatsPerVertex >= 6) {
        glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].floatsPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        offset += 3;
    }
    // Tangent/Bitangent
    if (objectEntity.objects[Index].floatsPerVertex >= offset + 6) {
        glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].floatsPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        offset += 3;
        glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].floatsPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        offset += 3;
    }
    // TexCoords
    if (objectEntity.objects[Index].TexCoords) { //* Only set if TexCoords is present
        glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].TexCoords * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        offset += 2;
    }
    // Vertex Color
    if (objectEntity.objects[Index].floatsPerVertex >= offset + 4) {
        glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, objectEntity.objects[Index].floatsPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        offset += 4;
    }

    glBindVertexArray(0); //* Unbind VAO
}

void Renderer::loadTexture(const std::string &texturePath, int Index)
{
    glGenTextures(1, &objectEntity.objects[Index].diffuseTextureID);
    glBindTexture(GL_TEXTURE_2D, objectEntity.objects[Index].diffuseTextureID);

    // Texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
        {
            std::cout << "Unsupported texture format: " << nrChannels << " channels\n";
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << texturePath << std::endl;
        objectEntity.objects[Index].diffuseTextureID = 0;
    }

    stbi_image_free(data);
}

void Renderer::shaderLoaderLight()
{
    std::string vertSourceLight = readFile(LIGHT_VERT_SHADER_PATH);
    std::string fragSourceLight = readFile(LIGHT_FRAG_SHADER_PATH);

    const char *vertexShaderSource = vertSourceLight.c_str();
    const char *fragmentShaderSource = fragSourceLight.c_str();

    lightData.vertexShaderLight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(lightData.vertexShaderLight, 1, &vertexShaderSource, NULL);
    glCompileShader(lightData.vertexShaderLight);
    glGetShaderiv(lightData.vertexShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(lightData.vertexShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    lightData.fragmentShaderLight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lightData.fragmentShaderLight, 1, &fragmentShaderSource, NULL);
    glCompileShader(lightData.fragmentShaderLight);
    glGetShaderiv(lightData.fragmentShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(lightData.fragmentShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    lightData.shaderProgramLight = glCreateProgram();
    glAttachShader(lightData.shaderProgramLight, lightData.vertexShaderLight);
    glAttachShader(lightData.shaderProgramLight, lightData.fragmentShaderLight);
    glLinkProgram(lightData.shaderProgramLight);
    glGetProgramiv(lightData.shaderProgramLight, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(lightData.shaderProgramLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(lightData.vertexShaderLight);
    glDeleteShader(lightData.fragmentShaderLight);
}

void Renderer::LightShaderLink()
{
    glGenVertexArrays(1, &lightData.lightCubeVAO);
    glGenBuffers(1, &lightData.lightCubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightData.lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);

    glBindVertexArray(lightData.lightCubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}



void Renderer::drawCallback()
{
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    cam.update();
    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 100.0f);

    for (size_t i = 0; i < objectEntity.objects.size(); ++i)
    {
        glUseProgram(objectEntity.objects[i].shaderProgram);
        ShaderHelper shader(objectEntity.objects[i].shaderProgram);

        // Bind textures and set uniforms
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].diffuseTextureID);
        shader.setInt("diffuse", 0);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].specularTextureID);
        // shader.setInt("material.specular", 1);
        // shader.setFloat("material.shininess", objectEntity.objects[i].material.shininess);

        // // Directional light
        // shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        // shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
        // shader.setVec3("dirLight.diffuse", glm::vec3(0.8f));
        // shader.setVec3("dirLight.specular", glm::vec3(1.0f));

        // // Point lights
        // for (int j = 0; j < 4; ++j)
        // {
        //     std::string idx = std::to_string(j);
        //     shader.setVec3("pointLights[" + idx + "].position", pointLightPositions[j]);
        //     shader.setVec3("pointLights[" + idx + "].ambient", glm::vec3(0.05f));
        //     shader.setVec3("pointLights[" + idx + "].diffuse", glm::vec3(0.8f));
        //     shader.setVec3("pointLights[" + idx + "].specular", glm::vec3(1.0f));
        //     shader.setFloat("pointLights[" + idx + "].constant", 1.0f);
        //     shader.setFloat("pointLights[" + idx + "].linear", 0.09f);
        //     shader.setFloat("pointLights[" + idx + "].quadratic", 0.032f);
        // }

        // // SpotLight
        // shader.setVec3("spotLight.position", cam.cameraPos);
        // shader.setVec3("spotLight.direction", cam.cameraFront);
        // shader.setVec3("spotLight.ambient", glm::vec3(0.0f));
        // shader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
        // shader.setVec3("spotLight.specular", glm::vec3(1.0f));
        // shader.setFloat("spotLight.constant", 1.0f);
        // shader.setFloat("spotLight.linear", 0.09f);
        // shader.setFloat("spotLight.quadratic", 0.032f);
        // shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(2.5f)));
        // shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.0f)));

        // // Set light properties
        // shader.setVec3("light.position", lightData.position);
        // shader.setVec3("light.ambient", glm::vec3(0.2f));
        // shader.setVec3("light.diffuse", glm::vec3(0.5f));
        // shader.setVec3("light.specular", glm::vec3(1.0f));
        shader.setMat4("view", cam.getViewMatrix());
        shader.setMat4("projection", projection);

        glm::mat4 modelMat = glm::mat4(1.0f);
        // You can set model transform per object here if needed
        shader.setMat4("model", modelMat);
        glBindVertexArray(objectEntity.objects[i].VAO);
        if (objectEntity.objects[i].indices && objectEntity.objects[i].indicesCount > 0) {
            glDrawElements(GL_TRIANGLES, objectEntity.objects[i].indicesCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, objectEntity.objects[i].vertexCount);
        }
    }

    // Animate and draw light
    glBindVertexArray(lightData.lightCubeVAO);
    glUseProgram(lightData.shaderProgramLight);
    ShaderHelper lightShader(lightData.shaderProgramLight);
    lightShader.setVec3("objectColor", lightData.lightColor);
    lightShader.setMat4("view", cam.getViewMatrix());
    lightShader.setMat4("projection", projection);
    glm::mat4 modelLight = glm::mat4(1.0f);

    modelLight = glm::translate(glm::mat4(1.0f), pointLightPositions[1]);
    lightShader.setMat4("model", modelLight);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::drawCleanup()
{
    for (size_t i = 0; i < objectEntity.objects.size(); ++i)
    {
        glDeleteVertexArrays(1, &objectEntity.objects[i].VAO);
        glDeleteBuffers(1, &objectEntity.objects[i].VBO);
        // Optionally delete objectEntity.objects[i].vertices if allocated with new
        delete[] objectEntity.objects[i].vertices;
    }
}