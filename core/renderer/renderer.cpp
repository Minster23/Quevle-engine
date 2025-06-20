// --- Includes ---
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/renderer/renderer.hpp>
#include <utils/Debug.h>
#include <utils/fileReader.hpp>

// --- Constants ---
namespace
{
    constexpr char VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/vertex.glsl";
    constexpr char FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/fragment.glsl";
    constexpr char LIGHT_VERT_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightVert.glsl";
    constexpr char LIGHT_FRAG_SHADER_PATH[] = "D:/QuavleEngine/utils/shader/lightFrag.glsl";
    constexpr char DIFFUSE_TEXTURE_PATH[] = "C:/Users/athilah/Downloads/container2.png";
    constexpr char SPECULAR_TEXTURE_PATH[] = "C:/Users/athilah/Downloads/container2_specular.png";
}

// --- Cube and Light Data ---

glm::vec3 lightPos(1.0f, 2.0f, 2.0f);

float vertices[] = {
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
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
// positions all containers
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};
// world space positions of our cubes

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};
// f

using namespace QuavleEngine;

WindowManager windowManager;

// --- Renderer Implementation ---

void Renderer::init()
{
    DEBUG_PRINT("Renderer::init() called");
    shaderLoader();
    shaderLink();
    shaderLoaderLight();
    LightShaderLink();
    objData.diffuseTextureID = loadTexture(DIFFUSE_TEXTURE_PATH);
    objData.specularTextureID = loadTexture(SPECULAR_TEXTURE_PATH);
    cam.init();
}

void Renderer::shaderLoader()
{
    DEBUG_PRINT("Renderer::shaderLoader() called");
    std::string vertSource = readFile(VERT_SHADER_PATH);
    std::string fragSource = readFile(FRAG_SHADER_PATH);

    const char *vertexShaderSource = vertSource.c_str();
    const char *fragmentShaderSource = fragSource.c_str();

    objData.vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(objData.vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(objData.vertexShader);
    glGetShaderiv(objData.vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objData.vertexShader, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    objData.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(objData.fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(objData.fragmentShader);
    glGetShaderiv(objData.fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(objData.fragmentShader, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    objData.shaderProgram = glCreateProgram();
    glAttachShader(objData.shaderProgram, objData.vertexShader);
    glAttachShader(objData.shaderProgram, objData.fragmentShader);
    glLinkProgram(objData.shaderProgram);
    glGetProgramiv(objData.shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(objData.shaderProgram, 512, NULL, infoLog);
        DEBUG_PRINT("ERROR::SHADER::LINKING_FAILED\n" + std::string(infoLog));
    }

    glDeleteShader(objData.vertexShader);
    glDeleteShader(objData.fragmentShader);
}

void Renderer::shaderLink()
{
    glGenVertexArrays(1, &objData.VAO);
    glGenBuffers(1, &objData.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, objData.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(objData.VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int Renderer::loadTexture(const std::string &texturePath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

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
            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << texturePath << std::endl;
        textureID = 0;
    }

    stbi_image_free(data);
    return textureID;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    glUseProgram(objData.shaderProgram);
    // Bind textures and set uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objData.diffuseTextureID);
    glUniform1i(glGetUniformLocation(objData.shaderProgram, "material.diffuse"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, objData.specularTextureID);
    glUniform1i(glGetUniformLocation(objData.shaderProgram, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "material.shininess"), 32.0f);

    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 100.0f);

    //!==========================================================

    // Directional light
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

    // Point lights
    for (int i = 0; i < 4; ++i) {
        std::string idx = std::to_string(i);
        glUniform3fv(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].position").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
        glUniform3f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].diffuse").c_str()), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].constant").c_str()), 1.0f);
        glUniform1f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].linear").c_str()), 0.09f);
        glUniform1f(glGetUniformLocation(objData.shaderProgram, ("pointLights[" + idx + "].quadratic").c_str()), 0.032f);
    }

    // SpotLight
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "spotLight.position"), 1, glm::value_ptr(cam.cameraPos)); // Camera position as spot light position
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "spotLight.direction"), 1, glm::value_ptr(cam.cameraFront));
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(objData.shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "spotLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "spotLight.quadratic"), 0.032f);
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(2.5f)));
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(17.0f)));

    //!==========================================================

    // Set light properties
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.position"), 1, glm::value_ptr(lightData.position));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(objData.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Animate and draw light
    glBindVertexArray(lightData.lightCubeVAO);
    glUseProgram(lightData.shaderProgramLight);
    glUniform3fv(glGetUniformLocation(lightData.shaderProgramLight, "objectColor"), 1, glm::value_ptr(lightData.lightColor));
    glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 modelLight = glm::mat4(1.0f);

    for (unsigned int i; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
    {
        modelLight = glm::translate(modelLight, glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z));
        glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Renderer::drawCleanup()
{
    glDeleteVertexArrays(1, &objData.VAO);
    glDeleteBuffers(1, &objData.VBO);
}