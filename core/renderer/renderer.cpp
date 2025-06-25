// --- Includes ---
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <filesystem>

#include <stb_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/renderer/renderer.hpp>
#include <core/interface/interface.hpp>
#include <core/renderer/shader_h.h>


// --- Constants ---
namespace
{
    constexpr char VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/vertex.glsl";
    constexpr char FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/fragment.glsl";

    constexpr char LIGHT_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightVert.glsl";
    constexpr char LIGHT_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightFrag.glsl";
    
    constexpr char DIFFUSE_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/ak-47_kalashnikov/textures/material_baseColor.png";
    constexpr char NORMAL_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/ak-47_kalashnikov/textures/material_normal.png";
    constexpr char METALLIC_TEXTURE_PATH[] = "F:/Devlopment/LLL/EmguCVApp/ak-47_kalashnikov/textures/material_metallicRoughness.png";
    constexpr char DIFFUSE_TEX_TEST[] = "F:/Devlopment/LLL/EmguCVApp/tex.png";
    constexpr char SPECULAR_TEXTURE_PATH[] = "C:/Users/athilah/Downloads/container2_specular.png";

    constexpr char SKYBOX_VERT_PATH[] = "D:/QuavleEngine/utils/shader/skyVert.glsl";
    constexpr char SKYBOX_FRAG_PATH[] = "D:/QuavleEngine/utils/shader/skyFrag.glsl";

    constexpr char GRID_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/gridVert.glsl";
    constexpr char GRID_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/gridFrag.glsl";
}

std::vector<std::string> faces{
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/right.jpg",
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/left.jpg",
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/top.jpg",
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/bottom.jpg",
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/front.jpg",
    "F:/Devlopment/LLL/EmguCVApp/skybox/skybox/back.jpg",
};

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

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

unsigned int gridShaderProgram;
GLuint gridVAO;
GLuint gridVBO;
GLuint gridEBO;

// Lights facing cubes, each 10 units away on the +X axis
using namespace QuavleEngine;

bool Renderer::Diffuse = true;
bool Renderer::Specular = true;
bool Renderer::Normal = true;
bool Renderer::Metallic = true;
bool Renderer::Roughness = true;
bool Renderer::Grid = true;

WindowManager windowManager;
interface intfc;
ObjectEntity objectEntity;
// --- Renderer Implementation ---

void Renderer::init()
{
    //* initialize the object entity
    cam.init();
    intfc.inputDebug("Info", "Renderer::init() called");
    
    shaderLoaderLight();
    LightShaderLink();

    objectEntity.firstCubemap();


    //* CUBEMAP
    shaderLoader(0, RenderType::SKYBOX);
    shaderLink(0, RenderType::SKYBOX);
    loadCubemapTexture(faces, 0);

    //* GRID
    createGridShaderProgram();
    setupGridQuad();
}

void Renderer::loadModelFirst()
{
    //* OBJECTS (kalo UI dah jadi hapus aja buat ulang yang kayak gini)
    Model model("F:/Devlopment/LLL/EmguCVApp/ak-47_kalashnikov/scene.gltf", true);
    for (size_t i = 0; i < objectEntity.objects.size(); ++i)
    {
        shaderLoader(i, RenderType::OBJECT);
        shaderLink(i, RenderType::OBJECT);
        // Load textures for each object (if you have per-object textures, set the path accordingly)
        loadTexture(DIFFUSE_TEXTURE_PATH, i, TextureType::DIFFUSE);
        loadTexture(NORMAL_TEXTURE_PATH, i, TextureType::NORMAL);
        loadTexture(METALLIC_TEXTURE_PATH, i, TextureType::METALLIC);
    }
}

void Renderer::LoadAnotherLight(){
    objectEntity.firstLightObject();
}

//* ====================== GRID SHADER ==========================
GLuint Renderer::compileShader(GLenum type, const char* source) {
    GLuint gridShaderProgram = glCreateShader(type);
    glShaderSource(gridShaderProgram, 1, &source, nullptr);
    glCompileShader(gridShaderProgram);

    GLint success;
    glGetShaderiv(gridShaderProgram, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(gridShaderProgram, 512, nullptr, info);
        std::cerr << "Shader error:\n" << info << std::endl;
    }
    return gridShaderProgram;
}

void Renderer::createGridShaderProgram()
{
    std::string vertSource = readFile(GRID_VERT_SHADER_PATH);
    std::string fragSource = readFile(GRID_FRAG_SHADER_PATH);
    const char *vertexShaderSource = vertSource.c_str();
    const char *fragmentShaderSource = fragSource.c_str();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    gridShaderProgram = glCreateProgram();
    glAttachShader(gridShaderProgram, vs);
    glAttachShader(gridShaderProgram, fs);
    glLinkProgram(gridShaderProgram);

    GLint success;
    glGetProgramiv(gridShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(gridShaderProgram, 512, nullptr, info);
        std::cerr << "Link error:\n" << info << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Renderer::setupGridQuad() {
    float quadVertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glGenBuffers(1, &gridEBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Renderer::shaderLoader(int Index, Renderer::RenderType expression)
{
    if (expression == RenderType::OBJECT)
    {
        //* Load and compile vertex and fragment shaders for the object
        intfc.inputDebug("Info", "Renderer::shaderLoader() called for OBJECT");
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
            intfc.inputDebug("Warning", "ERROR::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
        }

        // Fragment Shader
        objectEntity.objects[Index].fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(objectEntity.objects[Index].fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(objectEntity.objects[Index].fragmentShader);
        glGetShaderiv(objectEntity.objects[Index].fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(objectEntity.objects[Index].fragmentShader, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
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
            intfc.inputDebug("Warning", "ERROR::SHADER::LINKING_FAILED\n" + std::string(infoLog));
        }

        //* Clean up shaders after linking
        glDeleteShader(objectEntity.objects[Index].vertexShader);
        glDeleteShader(objectEntity.objects[Index].fragmentShader);
    }

    if (expression == RenderType::SKYBOX)
    {
        //* Load and compile vertex and fragment shaders for the cubemap
        intfc.inputDebug("Info", "Renderer::shaderLoader() called for SKYBOX");
        std::string vertSource = readFile(SKYBOX_VERT_PATH);
        std::string fragSource = readFile(SKYBOX_FRAG_PATH);
        const char *vertexShaderSource = vertSource.c_str();
        const char *fragmentShaderSource = fragSource.c_str();

        // Vertex Shader
        objectEntity.CubeMaps[Index].vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(objectEntity.CubeMaps[Index].vertex, 1, &vertexShaderSource, NULL);
        glCompileShader(objectEntity.CubeMaps[Index].vertex);
        glGetShaderiv(objectEntity.CubeMaps[Index].vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(objectEntity.CubeMaps[Index].vertex, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
        }

        // Fragment Shader
        objectEntity.CubeMaps[Index].fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(objectEntity.CubeMaps[Index].fragment, 1, &fragmentShaderSource, NULL);
        glCompileShader(objectEntity.CubeMaps[Index].fragment);
        glGetShaderiv(objectEntity.CubeMaps[Index].fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(objectEntity.CubeMaps[Index].fragment, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
        }

        // Shader Program
        objectEntity.CubeMaps[Index].shaderProgramCubemap = glCreateProgram();
        glAttachShader(objectEntity.CubeMaps[Index].shaderProgramCubemap, objectEntity.CubeMaps[Index].vertex);
        glAttachShader(objectEntity.CubeMaps[Index].shaderProgramCubemap, objectEntity.CubeMaps[Index].fragment);
        glLinkProgram(objectEntity.CubeMaps[Index].shaderProgramCubemap);
        glGetProgramiv(objectEntity.CubeMaps[Index].shaderProgramCubemap, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(objectEntity.CubeMaps[Index].shaderProgramCubemap, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::SHADER::LINKING_FAILED\n" + std::string(infoLog));
        }

        //* Clean up shaders after linking
        glDeleteShader(objectEntity.CubeMaps[Index].vertex);
        glDeleteShader(objectEntity.CubeMaps[Index].fragment);
    }
}

void Renderer::shaderLink(int Index, Renderer::RenderType expression)
{
    if (expression == RenderType::OBJECT)
    {
        intfc.inputDebug("Info", "Linking shader for OBJECT");
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
            GL_STATIC_DRAW);

        // Index buffer
        if (objectEntity.objects[Index].indices && objectEntity.objects[Index].indicesCount > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEntity.objects[Index].EBO);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                objectEntity.objects[Index].indicesCount * sizeof(unsigned int),
                objectEntity.objects[Index].indices,
                GL_STATIC_DRAW);
        }

        // Set up vertex attributes: position, normal, texcoord (fixed layout)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    if (expression == RenderType::SKYBOX)
    {
        intfc.inputDebug("Info", "Linking shader for SKYBOX");
        glGenVertexArrays(1, &objectEntity.CubeMaps[Index].cubemapVAO);
        glGenBuffers(1, &objectEntity.CubeMaps[Index].cubemapVBO);
        glBindVertexArray(objectEntity.CubeMaps[Index].cubemapVAO);
        glBindBuffer(GL_ARRAY_BUFFER, objectEntity.CubeMaps[Index].cubemapVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }

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
        GL_STATIC_DRAW);

    // Fill EBO if indices exist
    if (objectEntity.objects[Index].indices && objectEntity.objects[Index].indicesCount > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEntity.objects[Index].EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            objectEntity.objects[Index].indicesCount * sizeof(unsigned int),
            objectEntity.objects[Index].indices,
            GL_STATIC_DRAW);
    }

    // Set up vertex attributes: position, normal, texcoord
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// CubeMap Shader
void Renderer::loadCubemapTexture(const std::vector<std::string>& faces, int Index)
{
    unsigned int* texID = &objectEntity.CubeMaps[Index].textureID;
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false); // Cubemaps should not be flipped
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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
                format = GL_RGB;

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Setup VAO/VBO for skybox if needed
    glGenVertexArrays(1, &objectEntity.CubeMaps[Index].cubemapVAO);
    glGenBuffers(1, &objectEntity.CubeMaps[Index].cubemapVBO);
    glBindVertexArray(objectEntity.CubeMaps[Index].cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.CubeMaps[Index].cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Renderer::shaderLoaderLight()
{
    std::string vertSourceLight = readFile(LIGHT_VERT_SHADER_PATH);
    std::string fragSourceLight = readFile(LIGHT_FRAG_SHADER_PATH);

    const char *vertexShaderSource = vertSourceLight.c_str();
    const char *fragmentShaderSource = fragSourceLight.c_str();

    unsigned int vertexShaderLight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderLight, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderLight);
    glGetShaderiv(vertexShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShaderLight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderLight, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderLight);
    glGetShaderiv(fragmentShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    lightShaderProgram = glCreateProgram();
    glAttachShader(lightShaderProgram, vertexShaderLight);
    glAttachShader(lightShaderProgram, fragmentShaderLight);
    glLinkProgram(lightShaderProgram);
    glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(lightShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderLight);
    glDeleteShader(fragmentShaderLight);
}

void Renderer::LightShaderLink()
{
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);

    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Renderer::drawCallback()
{
    intfc.inputDebug("Info", "CallBack");
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    cam.update();
    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 1000.0f);

    if (objectEntity.objects.size() != 0)
    {
        for (size_t i = 0; i < objectEntity.objects.size(); ++i)
        {
            glUseProgram(objectEntity.objects[i].shaderProgram);
            ShaderHelper shader(objectEntity.objects[i].shaderProgram);

            // Bind diffuse texture and set uniform (standard, simple)
            // Bind and set diffuse texture
            if (Diffuse)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].diffuseTextureID);
                shader.setInt("diffuse", 0);
            }
            // Bind and set specular texture
            if (Specular)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].specularTextureID);
                shader.setInt("specular", 1);
            }

            // Bind and set normal map
            if (Normal)
            {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].normalTextureID);
                shader.setInt("normalMap", 2);
            }

            if (Metallic)
            {
                // Bind and set metallic map
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].metallicTextureID);
                shader.setInt("metallicMap", 3);
            }

            // Bind and set roughness map (if you have it)
            if (Roughness)
            {
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, objectEntity.objects[i].roughnessTextureID);
                shader.setInt("roughnessMap", 4);
            }

            // Send to shader
            shader.setInt("lightCount", objectEntity.lights.size());
            for (size_t light_idx = 0; light_idx < objectEntity.lights.size(); ++light_idx)
            {
                std::string index = std::to_string(light_idx);
                shader.setVec3("lightPositions[" + index + "]", objectEntity.lights[light_idx].position);
                shader.setVec3("lightColors[" + index + "]", objectEntity.lights[light_idx].lightColor);
                shader.setFloat("lightIntensities[" + index + "]", objectEntity.lights[light_idx].intensity); // Set light intensity for each light
            }

            shader.setMat4("view", cam.getViewMatrix());
            shader.setMat4("projection", projection);

            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::scale(modelMat, objectEntity.objects[i].scale);
            modelMat = glm::translate(modelMat, objectEntity.objects[i].position);
            glm::vec3 rot = glm::radians(objectEntity.objects[i].rotation);

            modelMat = glm::rotate(modelMat, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMat = glm::rotate(modelMat, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMat = glm::rotate(modelMat, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
            shader.setMat4("model", modelMat);

            shader.setVec3("viewPos", cam.cameraPos);

            glBindVertexArray(objectEntity.objects[i].VAO);
            if (objectEntity.objects[i].indices && objectEntity.objects[i].indicesCount > 0 && objectEntity.objects[i].isShow)
            {
                glDrawElements(GL_TRIANGLES, objectEntity.objects[i].indicesCount, GL_UNSIGNED_INT, 0);
            }
            else
            {
                glDrawArrays(GL_TRIANGLES, 0, objectEntity.objects[i].vertexCount);
            }
        }
    }
    else
    {
    }

    if (!objectEntity.lights.empty())
    {
        glUseProgram(lightShaderProgram);
        ShaderHelper lightShader(lightShaderProgram);
        lightShader.setMat4("view", cam.getViewMatrix());
        lightShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < objectEntity.lights.size(); ++i)
        {
            glm::mat4 modelLight = glm::mat4(1.0f);
            modelLight = glm::translate(modelLight, objectEntity.lights[i].position);
            lightShader.setMat4("model", modelLight);
            lightShader.setVec3("objectColor", objectEntity.lights[i].lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    if (Grid)
    {
        glUseProgram(gridShaderProgram);
        ShaderHelper gridShader(gridShaderProgram);
        gridShader.setMat4("view", cam.getViewMatrix());
        gridShader.setMat4("projection", projection);
        gridShader.setVec3("camPos", cam.cameraPos);
        glBindVertexArray(gridVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glDepthFunc(GL_LEQUAL);
    ShaderHelper skyboxShader(objectEntity.CubeMaps[0].shaderProgramCubemap);
    glUseProgram(objectEntity.CubeMaps[0].shaderProgramCubemap);
    glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMatrix()));
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    glBindVertexArray(objectEntity.CubeMaps[0].cubemapVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, objectEntity.CubeMaps[0].textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
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