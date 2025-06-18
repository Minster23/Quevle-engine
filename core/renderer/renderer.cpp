#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/renderer/renderer.hpp>
#include <fstream>
#include <sstream>
#include <utils/Debug.h>
#include <utils/fileReader.hpp>


glm::vec3 lightPos(1.2f, 6.0f, 2.0f);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
// world space positions of our cubes

using namespace QuavleEngine;

WindowManager windowManager;
// camera camR; // Remove global camera instance, it's now a member of Renderer

void Renderer::init()
{
    DEBUG_PRINT("Renderer::init() called");
    shaderLoader(); //* Memuat Shader
    shaderLink();   //* Menghubungkan Shader
    LightShader();
    shaderLoaderLight(); //* Memuat Shader untuk Lampu
    loadTexture(); //* Memuat Texture
    cam.init();    //* Memuat/Menyiapkan camera
}


void Renderer::shaderLoader()
{
    DEBUG_PRINT("Renderer::shaderLoader() called");
    std::string vertSource = readFile("D:/QuavleEngine/utils/shader/vertex.glsl");
    std::string fragSource = readFile("D:/QuavleEngine/utils/shader/fragment.glsl");

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

    glBindVertexArray(objData.VAO);


    glBindBuffer(GL_ARRAY_BUFFER, objData.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(objData.VBO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Renderer::loadTexture()
{
    // Generate and bind texture
    glGenTextures(1, &objData.textureID);
    glBindTexture(GL_TEXTURE_2D, objData.textureID);

    // Texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string texturePath = "D:/ReaperMedia/Compositing/Media/tex.png";
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
    }

    stbi_image_free(data);
}

void Renderer::shaderLoaderLight()
{
    std::string vertSource = readFile("D:/QuavleEngine/utils/shader/lightVert.glsl");
    std::string fragSource = readFile("D:/QuavleEngine/utils/shader/lightFrag.glsl");

    const char *vertexShaderSource = vertSource.c_str();
    const char *fragmentShaderSource = fragSource.c_str();

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

    unsigned int shaderProgramLight = glCreateProgram();
    glAttachShader(shaderProgramLight, vertexShaderLight);
    glAttachShader(shaderProgramLight, fragmentShaderLight);
    glLinkProgram(shaderProgramLight);
    glGetProgramiv(shaderProgramLight, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderLight);
    glDeleteShader(fragmentShaderLight);
}

void Renderer::LightShader()
{
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void Renderer::drawCallback()
{
    DEBUG_PRINT("Renderer::drawCallback() called");
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    cam.update(); //* Update camera position and view matrix

    glUseProgram(objData.shaderProgram);
    // Set the texture for shaderProgram
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objData.textureID);
    glUniform1i(glGetUniformLocation(objData.shaderProgram, "texture1"), 0);

    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "viewPos"), 1, glm::value_ptr(cam.cameraPos));

    // Set material properties
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "material.shininess"), 32.0f);

    // Set light properties
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.position"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    // Set the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 100.0f);
    // Use the view matrix from the camera object
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Draw 10 cubes
    glBindVertexArray(objData.VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Render with shaderProgramLight
    glUseProgram(shaderProgramLight);
    glm::mat4 modelLight = glm::mat4(1.0f);
    modelLight = glm::translate(modelLight, lightPos);
    modelLight = glm::scale(modelLight, glm::vec3(0.2f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramLight, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramLight, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramLight, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    float time = glfwGetTime();
    lightPos.x = 5.0f * cos(time);
    lightPos.z = 5.0f * sin(time);
}

void Renderer::drawCleanup()
{
    glDeleteVertexArrays(1, &objData.VAO);
    glDeleteBuffers(1, &objData.VBO);
}