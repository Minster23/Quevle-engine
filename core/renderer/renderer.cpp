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
    constexpr char DIFFUSE_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/hk_usp_9mm_pistol/textures/USP9_baseColor.png";
    constexpr char NORMAL_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/hk_usp_9mm_pistol/textures/USP9_normal.png";
    constexpr char METALLIC_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/hk_usp_9mm_pistol/textures/USP9_metallicRoughness.png";
    constexpr char DIFFUSE_TEX_TEST[] = "F:/Devlopment/LLL/EmguCVApp/tex.png";
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

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
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
    //* initialize the object entity
    DEBUG_PRINT("Renderer::init() called");
    Model model("F:/Devlopment/LLL/EmguCVApp/hk_usp_9mm_pistol/scene.gltf", true);
    objectEntity.firstLightObject();

    //* do something with the model
    objectEntity.objects = ObjectEntity::objects;
    shaderLoaderLight();
    LightShaderLink();
    cam.init();
    for (size_t i = 0; i < objectEntity.objects.size(); ++i) {
        shaderLoader(i);
        shaderLink(i);
        // Load textures for each object (if you have per-object textures, set the path accordingly)
        loadTexture(DIFFUSE_TEXTURE_PATH, i, TextureType::DIFFUSE);
        loadTexture(NORMAL_TEXTURE_PATH, i, TextureType::NORMAL);
        loadTexture(METALLIC_TEXTURE_PATH, i, TextureType::METALLIC);
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
    // Generate and bind VAO, VBO, and EBO for the object
    glGenVertexArrays(1, &objectEntity.objects[Index].VAO);
    glGenBuffers(1, &objectEntity.objects[Index].VBO);
    glGenBuffers(1, &objectEntity.objects[Index].EBO);
    glBindVertexArray(objectEntity.objects[Index].VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.objects[Index].VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        objectEntity.objects[Index].vertexCount * 8 * sizeof(float), // always 8 floats per vertex
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

    // Set up vertex attributes: position, normal, texcoord (fixed layout)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
}

void Renderer::loadTexture(const std::string &texturePath, int Index, TextureType expression)
{
    unsigned int *texID = nullptr;
    switch (expression)
    {
    case Renderer::TextureType::DIFFUSE:
        texID = &objectEntity.objects[Index].diffuseTextureID;
        break;
    case Renderer::TextureType::SPECULAR:
        texID = &objectEntity.objects[Index].specularTextureID;
        break;
    case Renderer::TextureType::NORMAL:
        texID = &objectEntity.objects[Index].normalTextureID;
        break;
    case Renderer::TextureType::METALLIC:
        texID = &objectEntity.objects[Index].metallicTextureID;
        break;
    default:
        break;
    }

    if (!texID) return;

    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

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
        *texID = 0;
    }

    stbi_image_free(data);

    // Fill VAO, VBO, EBO if not already filled
    if (objectEntity.objects[Index].VAO == 0)
        glGenVertexArrays(1, &objectEntity.objects[Index].VAO);
    if (objectEntity.objects[Index].VBO == 0)
        glGenBuffers(1, &objectEntity.objects[Index].VBO);
    if (objectEntity.objects[Index].EBO == 0)
        glGenBuffers(1, &objectEntity.objects[Index].EBO);

    glBindVertexArray(objectEntity.objects[Index].VAO);

    // Fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.objects[Index].VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        objectEntity.objects[Index].vertexCount * 8 * sizeof(float),
        objectEntity.objects[Index].vertices,
        GL_STATIC_DRAW
    );

    // Fill EBO if indices exist
    if (objectEntity.objects[Index].indices && objectEntity.objects[Index].indicesCount > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEntity.objects[Index].EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            objectEntity.objects[Index].indicesCount * sizeof(unsigned int),
            objectEntity.objects[Index].indices,
            GL_STATIC_DRAW
        );
    }

    // Set up vertex attributes: position, normal, texcoord
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::shaderLoaderLight()
{
    std::string vertSourceLight = readFile(LIGHT_VERT_SHADER_PATH);
    std::string fragSourceLight = readFile(LIGHT_FRAG_SHADER_PATH);

    const char *vertexShaderSource = vertSourceLight.c_str();
    const char *fragmentShaderSource = fragSourceLight.c_str();

    objectEntity.lights[0].vertexShaderLight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(objectEntity.lights[0].vertexShaderLight, 1, &vertexShaderSource, NULL);
    glCompileShader(objectEntity.lights[0].vertexShaderLight);
    glGetShaderiv(objectEntity.lights[0].vertexShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objectEntity.lights[0].vertexShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    objectEntity.lights[0].fragmentShaderLight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(objectEntity.lights[0].fragmentShaderLight, 1, &fragmentShaderSource, NULL);
    glCompileShader(objectEntity.lights[0].fragmentShaderLight);
    glGetShaderiv(objectEntity.lights[0].fragmentShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objectEntity.lights[0].fragmentShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    objectEntity.lights[0].shaderProgramLight = glCreateProgram();
    glAttachShader(objectEntity.lights[0].shaderProgramLight, objectEntity.lights[0].vertexShaderLight);
    glAttachShader(objectEntity.lights[0].shaderProgramLight, objectEntity.lights[0].fragmentShaderLight);
    glLinkProgram(objectEntity.lights[0].shaderProgramLight);
    glGetProgramiv(objectEntity.lights[0].shaderProgramLight, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(objectEntity.lights[0].shaderProgramLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(objectEntity.lights[0].vertexShaderLight);
    glDeleteShader(objectEntity.lights[0].fragmentShaderLight);
}

void Renderer::LightShaderLink()
{
    glGenVertexArrays(1, &objectEntity.lights[0].lightCubeVAO);
    glGenBuffers(1, &objectEntity.lights[0].lightCubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.lights[0].lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);

    glBindVertexArray(objectEntity.lights[0].lightCubeVAO);
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
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 1000.0f);

    // Animate and draw light
    // Animate light position in an orbital path around (0, 0, 0)
    static float angle = 0.0f;
    angle += 0.001f; // Adjust speed as needed
    float radius = 5.0f;
    glm::vec3 lightPos(
        radius * cos(angle),
        1.0f, // fixed height
        radius * sin(angle));

    for (size_t i = 0; i < objectEntity.objects.size(); ++i)
    {
        glUseProgram(objectEntity.objects[i].shaderProgram);
        ShaderHelper shader(objectEntity.objects[i].shaderProgram);

        // Bind diffuse texture and set uniform (standard, simple)
        // Bind and set diffuse texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].diffuseTextureID);
        shader.setInt("diffuse", 0);

        // Bind and set specular texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].specularTextureID);
        shader.setInt("specular", 1);

        // Bind and set normal map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].normalTextureID);
        shader.setInt("normalMap", 2);

        // Bind and set metallic map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].metallicTextureID);
        shader.setInt("metallicMap", 3);

        // Bind and set roughness map (if you have it)
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].roughnessTextureID);
        shader.setInt("roughnessMap", 4);

        shader.setFloat("lightIntensity", 20.0f); // Set light intensity

        // Set view and projection matrices
        shader.setMat4("view", cam.getViewMatrix());
        shader.setMat4("projection", projection);

        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::scale(modelMat, glm::vec3(0.1f, 0.1f, 0.1f));
        shader.setMat4("model", modelMat);

        // Set light and view position
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", cam.cameraPos);

        glBindVertexArray(objectEntity.objects[i].VAO);
        if (objectEntity.objects[i].indices && objectEntity.objects[i].indicesCount > 0) {
            glDrawElements(GL_TRIANGLES, objectEntity.objects[i].indicesCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, objectEntity.objects[i].vertexCount);
        }
    }

    for (unsigned int i = 0; i < objectEntity.lights.size(); ++i)
    {
        if (objectEntity.lights[i].shaderProgramLight != 0)
        {

            glBindVertexArray(objectEntity.lights[0].lightCubeVAO);
            glUseProgram(objectEntity.lights[0].shaderProgramLight);
            ShaderHelper lightShader(objectEntity.lights[0].shaderProgramLight);
            lightShader.setVec3("objectColor", objectEntity.lights[0].lightColor);
            lightShader.setMat4("view", cam.getViewMatrix());
            lightShader.setMat4("projection", projection);

            glm::mat4 modelLight = glm::mat4(1.0f);
            modelLight = glm::translate(modelLight, lightPos);
            lightShader.setMat4("model", modelLight);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            continue;
        }
    }
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