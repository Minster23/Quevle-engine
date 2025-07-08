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

#include <utils/camera/camera.hpp>
#include <core/renderer/renderer.hpp>
#include <core/interface/interface.hpp>
#include <core/renderer/shader_h.h>

#include <future>
#include <thread>
#include <chrono>
#include <memory>

// --- Constants ---
namespace
{
    constexpr char VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/vertex.glsl";
    constexpr char FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/fragment.glsl";

    constexpr char LIGHT_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightVert.glsl";
    constexpr char LIGHT_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightFrag.glsl";

    constexpr char SKYBOX_VERT_PATH[] = "D:/QuavleEngine/utils/shader/skyVert.glsl";
    constexpr char SKYBOX_FRAG_PATH[] = "D:/QuavleEngine/utils/shader/skyFrag.glsl";

    constexpr char GRID_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/gridVert.glsl";
    constexpr char GRID_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/gridFrag.glsl";

    constexpr char BILLBOARD_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/billboardVert.glsl";
    constexpr char BILLBOARD_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/billboardFrag.glsl";

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

glm::mat4 Renderer::projection;
glm::mat4 Renderer::view;
glm::mat4 Renderer::model;

glm::mat4 Renderer::modelLight;

glm::vec3 Renderer::gridColor = glm::vec3(1.0f);
float Renderer::gridSpacing = 2.0f;


namespace QuavleEngine
{ // Explicitly define within the namespace
    WindowManager windowManager;
    interface intfc;
    ObjectEntity objectEntity;
}

//* ====================== Renderer Implementation ======================
void Renderer::init()
{
    glEnable(GL_MULTISAMPLE);
    //* initialize the object entity
    intfc.inputDebug("Info", "Engine loading any assets");

    //* Lights
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

void Renderer::loadModelFirst(std::string path)
{
    
    Model mode(path, true);
    for (size_t i = 0; i < objectEntity.objects.size(); ++i)
    {
        intfc.inputDebug("Info", "loading model");
        shaderLoader(i, RenderType::OBJECT);
        shaderLink(i, RenderType::OBJECT);
    }
}

void Renderer::LoadAnotherLight()
{
    intfc.inputDebug("Info", "loading Light");

    objectEntity.firstLightObject();
}

void Renderer::loadBillboard(){
    intfc.inputDebug("Info", "loading Billboard");
    objectEntity.firstBillboard();
    for (size_t i = 0; i < objectEntity.billboards.size(); ++i)
    {
        intfc.inputDebug("Info", "loading Billboard");
        shaderLoader(i, RenderType::BILLBOARD);
        loadTexture("F:/Devlopment/LLL/EmguCVApp/tex.png", i, TextureType::BILLBOARD);
        shaderLink(i, RenderType::BILLBOARD);
    }
}

//* ====================== GRID SHADER ==========================
GLuint Renderer::compileShader(GLenum type, const char *source)
{
    GLuint gridShaderProgram = glCreateShader(type);
    glShaderSource(gridShaderProgram, 1, &source, nullptr);
    glCompileShader(gridShaderProgram);

    GLint success;
    glGetShaderiv(gridShaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetShaderInfoLog(gridShaderProgram, 512, nullptr, info);
        std::cerr << "Shader error:\n"
                  << info << std::endl;
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
    if (!success)
    {
        char info[512];
        glGetProgramInfoLog(gridShaderProgram, 512, nullptr, info);
        std::cerr << "Link error:\n"
                  << info << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Renderer::setupGridQuad()
{
    float quadVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3};

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glGenBuffers(1, &gridEBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

void Renderer::shaderLoader(int Index, Renderer::RenderType expression)
{
    if (expression == RenderType::OBJECT)
    { // Declare vertSource here
        intfc.inputDebug("Info", "Renderer::shaderLoader() called for OBJECT");
        std::string fragSource;
        
        if (!objectEntity.objects[Index].hasOwnTexture) {
            fragSource = readFile(FRAG_SHADER_PATH);
        }else{
            fragSource = readFile(objectEntity.objects[Index].shaderLocation);
        }

        std::string vertSource = readFile(VERT_SHADER_PATH);
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

    if (expression == RenderType::BILLBOARD)
    {
        //* Load and compile vertex and fragment shaders for the cubemap
        intfc.inputDebug("Info", "Renderer::shaderLoader() called for BILLBOARD");
        std::string vertSource = readFile(BILLBOARD_VERT_SHADER_PATH);
        std::string fragSource = readFile(BILLBOARD_FRAG_SHADER_PATH);
        const char *vertexShaderSource = vertSource.c_str();
        const char *fragmentShaderSource = fragSource.c_str();

        // Vertex Shader
        objectEntity.billboards[Index].vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(objectEntity.billboards[Index].vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(objectEntity.billboards[Index].vertexShader);
        glGetShaderiv(objectEntity.billboards[Index].vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(objectEntity.billboards[Index].vertexShader, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
        }

        // Fragment Shader
        objectEntity.billboards[Index].fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(objectEntity.billboards[Index].fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(objectEntity.billboards[Index].fragmentShader);
        glGetShaderiv(objectEntity.billboards[Index].fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(objectEntity.billboards[Index].fragmentShader, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
        }

        // Shader Program
        objectEntity.billboards[Index].shaderProgram = glCreateProgram();
        glAttachShader(objectEntity.billboards[Index].shaderProgram, objectEntity.billboards[Index].vertexShader);
        glAttachShader(objectEntity.billboards[Index].shaderProgram, objectEntity.billboards[Index].fragmentShader);
        glLinkProgram(objectEntity.billboards[Index].shaderProgram);
        glGetProgramiv(objectEntity.billboards[Index].shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(objectEntity.billboards[Index].shaderProgram, 512, NULL, infoLog);
            intfc.inputDebug("Warning", "ERROR::SHADER::LINKING_FAILED\n" + std::string(infoLog));
        }

        //* Clean up shaders after linking
        glDeleteShader(objectEntity.billboards[Index].vertexShader);
        glDeleteShader(objectEntity.billboards[Index].fragmentShader);
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
        glBindVertexArray(0); // Unbind VAO
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
        glBindVertexArray(0); // Unbind VAO
    }
if (expression == RenderType::BILLBOARD)
{
    intfc.inputDebug("Info", "Linking shader for BILLBOARD");

    glGenVertexArrays(1, &objectEntity.billboards[Index].VAO);
    glGenBuffers(1, &objectEntity.billboards[Index].VBO);

    glBindVertexArray(objectEntity.billboards[Index].VAO);
    glBindBuffer(GL_ARRAY_BUFFER, objectEntity.billboards[Index].VBO);
    
    glBufferData(GL_ARRAY_BUFFER, 
                sizeof(float) * objectEntity.billboards[Index].vertices.size(),
                objectEntity.billboards[Index].vertices.data(), 
                GL_STATIC_DRAW);

    // Position attribute: location = 0, 3 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 
        5 * sizeof(float), (void*)0
    );

    // Texture coordinate attribute: location = 1, 2 floats
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 
        5 * sizeof(float), (void*)(3 * sizeof(float))
    );

    glBindVertexArray(0);
}

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
    case Renderer::TextureType::ROUGHNESS:
        texID = &objectEntity.objects[Index].roughnessTextureID;
        break;
    case Renderer::TextureType::HEIGHT:
        texID = &objectEntity.objects[Index].HeightTextureID;
        break;
    case Renderer::TextureType::AMBIENT:
        texID = &objectEntity.objects[Index].ambientTextureID;
        break;
    case Renderer::TextureType::EMISSIVE:
        texID = &objectEntity.objects[Index].emissiveTextureID;
        break;
    case Renderer::TextureType::OPACITY:
        texID = &objectEntity.objects[Index].opacityTextureID;
        break;
    case Renderer::TextureType::DISPLACEMENT:
        texID = &objectEntity.objects[Index].displacementTextureID;
        break;
    case Renderer::TextureType::BILLBOARD:
        texID = &objectEntity.billboards[Index].textureID;
        break;
    default:
        std::cerr << "Invalid texture type specified.\n";
        return;
    }

    if (!texID)
        return;

    // Generate and bind texture
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

    // Texture parameters
    constexpr GLint wrapMode = GL_REPEAT;
    constexpr GLint minFilter = GL_LINEAR_MIPMAP_LINEAR;
    constexpr GLint magFilter = GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported texture format: " << nrChannels << " channels\n";
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << texturePath << std::endl;
        *texID = 0;
    }

    stbi_image_free(data);

    if (expression == Renderer::TextureType::BILLBOARD)
    {
        auto &obj = objectEntity.billboards[Index];

        // Generate VAO/VBO if not created
        if (obj.VAO == 0)
            glGenVertexArrays(1, &obj.VAO);
        if (obj.VBO == 0)
            glGenBuffers(1, &obj.VBO);

        glBindVertexArray(obj.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);

        // Upload vertex data (position + texcoord = 5 floats per vertex)
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * obj.vertices.size(),
                     obj.vertices.data(),
                     GL_STATIC_DRAW);

        // Setup vertex attributes
        // Location 0: position (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

        // Location 1: texcoords (vec2)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    else
    {
        auto &obj = objectEntity.objects[Index];

        // Generate VAO/VBO/EBO if not created
        if (obj.VAO == 0)
            glGenVertexArrays(1, &obj.VAO);
        if (obj.VBO == 0)
            glGenBuffers(1, &obj.VBO);
        if (obj.EBO == 0)
            glGenBuffers(1, &obj.EBO);

        glBindVertexArray(obj.VAO);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
        glBufferData(GL_ARRAY_BUFFER, obj.vertexCount * 8 * sizeof(float), obj.vertices, GL_STATIC_DRAW);

        // Upload index data if available
        if (obj.indices && obj.indicesCount > 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indicesCount * sizeof(unsigned int), obj.indices, GL_STATIC_DRAW);
        }

        // Vertex attribute pointers are set in shaderLink for objects
        glBindVertexArray(0);
    }
}

//* ====================== CubeMap Shader ======================
void Renderer::loadCubemapTexture(const std::vector<std::string> &faces, int Index)
{
    unsigned int *texID = &objectEntity.CubeMaps[Index].textureID;
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false); // Cubemaps should not be flipped
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

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
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    projection = glm::perspective(glm::radians(cameras[cameraIndex].fov), aspectRatio, 0.1f, 1000.0f);

    //* Update matriks
    cameras[cameraIndex].view = glm::lookAt(
        cameras[cameraIndex].cameraPos,
        cameras[cameraIndex].cameraPos + cameras[cameraIndex].cameraFront,
        cameras[cameraIndex].cameraUp);

    auto bindTexture = [](GLuint texID, int unit, const std::string &name, ShaderHelper &shader)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texID);
        shader.setInt(name, unit);
    };

    if (Grid)
    {
        glEnable(GL_DEPTH_TEST); // just to be explicit
        glDepthMask(GL_TRUE);    // ensure depth writing
        glUseProgram(gridShaderProgram);

        ShaderHelper gridShader(gridShaderProgram);
        gridShader.setMat4("view", cameras[cameraIndex].view);
        gridShader.setMat4("projection", projection);
        gridShader.setVec3("camPos", cameras[cameraIndex].cameraPos);
        gridShader.setVec3("gridColor", gridColor);
        gridShader.setFloat("gridSpacing", gridSpacing);

        glBindVertexArray(gridVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    if (objectEntity.billboards.size() > 0)
    {
        glUseProgram(objectEntity.billboards[0].shaderProgram); // Use the first billboard's shader

        for (size_t i = 0; i < objectEntity.billboards.size(); ++i)
        {
            if (!objectEntity.billboards[i].isShow)
                continue;

            ShaderHelper shader(objectEntity.billboards[i].shaderProgram);
            shader.setMat4("view", cameras[cameraIndex].view);
            shader.setMat4("projection", projection);
            shader.setVec3("billboardPos", objectEntity.billboards[i].position);
            shader.setVec3("cameraPos", cameras[cameraIndex].cameraPos);

            bindTexture(objectEntity.billboards[i].textureID, 0, "texture1", shader);

            glBindVertexArray(objectEntity.billboards[i].VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices for two triangles
        }
    }

    if (objectEntity.objects.size() != 0)
    {
        int currentShader = -1;
        GLuint lastVAO = 0;


        for (size_t i = 0; i < objectEntity.objects.size(); ++i)
        {
            const auto &obj = objectEntity.objects[i];

            // Use program only if different
            if ((int)obj.shaderProgram != currentShader)
            {
                glUseProgram(obj.shaderProgram);
                currentShader = obj.shaderProgram;
            }

            ShaderHelper shader(obj.shaderProgram);

            // Set view/projection/camera once per shader
            shader.setMat4("view", cameras[cameraIndex].view);
            shader.setMat4("projection", projection);
            shader.setVec3("viewPos", cameras[cameraIndex].cameraPos);

            // Material textures
            if (Diffuse)
                bindTexture(obj.diffuseTextureID, 0, "diffuse", shader);
            if (Specular)
                bindTexture(obj.specularTextureID, 1, "specular", shader);
            if (Normal)
                bindTexture(obj.normalTextureID, 2, "normalMap", shader);
            if (Metallic)
                bindTexture(obj.metallicTextureID, 3, "metallicMap", shader);
            if (Roughness)
                bindTexture(obj.roughnessTextureID, 4, "roughnessMap", shader);

            bindTexture(obj.HeightTextureID, 5, "heightMap", shader);
            bindTexture(obj.ambientTextureID, 6, "ambientMap", shader);
            bindTexture(obj.emissiveTextureID, 7, "emissiveMap", shader);
            bindTexture(obj.opacityTextureID, 8, "opacityMap", shader);
            bindTexture(obj.displacementTextureID, 9, "displacementMap", shader);

            // Material data (.mtl style)
            shader.setVec3("mat_ambient", obj.material.ambient);
            shader.setVec3("mat_diffuse", obj.material.diffuse);
            shader.setVec3("mat_specular", obj.material.specular);
            shader.setVec3("mat_emissive", obj.material.emissive);
            shader.setFloat("mat_shininess", obj.material.shininess);
            shader.setFloat("mat_opacity", obj.material.opacity);
            shader.setFloat("mat_shininessStrength", obj.material.shininessStrength);

            // Lights
            shader.setInt("lightCount", objectEntity.lights.size());
            for (size_t light_idx = 0; light_idx < objectEntity.lights.size(); ++light_idx)
            {
                const auto &light = objectEntity.lights[light_idx];
                std::string index = std::to_string(light_idx);
                if (!objectEntity.lights[light_idx].isShow)
                {
                    shader.setFloat("lightIntensities[" + index + "]", 0.0f); // Set intensity to 0 if not shown
                } else
                {
                    shader.setVec3("lightPositions[" + index + "]", light.position);
                    shader.setVec3("lightColors[" + index + "]", light.lightColor);
                    shader.setFloat("lightIntensities[" + index + "]", light.intensity);
                }
            }

            // Model matrix
            model = glm::mat4(1.0f);

            // Apply translation first
            model = glm::translate(model, obj.position);

            // Then apply rotation (in radians)
            glm::vec3 rot = glm::radians(obj.rotation);
            model = glm::rotate(model, rot.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, rot.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, rot.z, glm::vec3(0, 0, 1));

            // Finally apply scale
            model = glm::scale(model, obj.scale);

            shader.setMat4("model", model);

            // Optional: selection highlight
            shader.setBool("selected", obj.isSelected);
            shader.setVec3("colorSelected", glm::vec3(1,1,0));

            // VAO binding only if changed
            if (obj.VAO != lastVAO)
            {
                glBindVertexArray(obj.VAO);
                lastVAO = obj.VAO;
            }

            // Draw
            if (obj.isShow)
            {
                if (obj.indices && obj.indicesCount > 0)
                    glDrawElements(GL_TRIANGLES, obj.indicesCount, GL_UNSIGNED_INT, 0);
                else
                    glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
            }
        }
    }

    if (!objectEntity.lights.empty())
    {
        glUseProgram(lightShaderProgram);
        ShaderHelper lightShader(lightShaderProgram);
        lightShader.setMat4("view", cameras[cameraIndex].view);
        lightShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < objectEntity.lights.size(); ++i)
        {
            if (objectEntity.lights[i].isShow)
            {
                modelLight = glm::mat4(1.0f);
                modelLight = glm::translate(modelLight, objectEntity.lights[i].position);
                lightShader.setMat4("model", modelLight);
                lightShader.setVec3("objectColor", objectEntity.lights[i].lightColor);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glDepthFunc(GL_LEQUAL);
    ShaderHelper skyboxShader(objectEntity.CubeMaps[0].shaderProgramCubemap);
    glUseProgram(objectEntity.CubeMaps[0].shaderProgramCubemap);
    view = glm::mat4(glm::mat3(cameras[cameraIndex].view));
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