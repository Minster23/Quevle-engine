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
// world space positions of our cubes

using namespace QuavleEngine;

WindowManager windowManager;
// camera camR; // Remove global camera instance, it's now a member of Renderer

void Renderer::init()
{
    DEBUG_PRINT("Renderer::init() called");
    shaderLoader(); //* Memuat Shader
    shaderLink();   //* Menghubungkan Shader
    shaderLoaderLight(); //* Memuat Shader untuk Lampu
    LightShaderLink();
    objData.diffuseTextureID = loadTexture("C:/Users/athilah/Downloads/container2.png"); // Initialize diffuse texture
    objData.specularTextureID = loadTexture("C:/Users/athilah/Downloads/container2_specular.png"); // Initialize specular texture
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

    glBindBuffer(GL_ARRAY_BUFFER, objData.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(objData.VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int Renderer::loadTexture(const std::string& texturePath)
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
    std::string vertSourceLight = readFile("D:/QuavleEngine/utils/shader/lightVert.glsl");
    std::string fragSourceLight = readFile("D:/QuavleEngine/utils/shader/lightFrag.glsl");

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Renderer::drawCallback()
{
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    cam.update(); //* Update camera position and view matrix

    glUseProgram(objData.shaderProgram);
    // Set the texture for shaderProgram
    // Bind diffuse texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objData.diffuseTextureID);
    glUniform1i(glGetUniformLocation(objData.shaderProgram, "material.diffuse"), 0);

    // Bind specular texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, objData.specularTextureID);
    glUniform1i(glGetUniformLocation(objData.shaderProgram, "material.specular"), 1);

    // Set material properties
    glUniform1f(glGetUniformLocation(objData.shaderProgram, "material.shininess"), 32.0f);

    // Set light properties
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.position"), 1, glm::value_ptr(lightData.position));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    glUniform3fv(glGetUniformLocation(objData.shaderProgram, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);
    // Set the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspectRatio, 0.1f, 100.0f);
    // Use the view matrix from the camera object
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(objData.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(objData.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // Ensure lightCubeVAO is properly bound and configured
    glBindVertexArray(lightData.lightCubeVAO);

    // Animate the light position along the X axis between -5 and 5 using deltaTime
    static float lightAnimX = -5.0f;
    static float direction = 1.0f;
    static float speed = 2.0f; // units per second
    static float lastTime = glfwGetTime();

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    lightAnimX += direction * speed * deltaTime;
    if (lightAnimX > 5.0f) {
        lightAnimX = 5.0f;
        direction = -1.0f;
    } else if (lightAnimX < -5.0f) {
        lightAnimX = -5.0f;
        direction = 1.0f;
    }
    lightData.position.x = lightAnimX;

    // Use the shader program for the light object
    glUseProgram(lightData.shaderProgramLight);

    // Set transformation matrices for the light object
    glm::mat4 modelLight = glm::mat4(1.0f);
    modelLight = glm::translate(modelLight, lightData.position); // Use animated position
    glUniform3fv(glGetUniformLocation(lightData.shaderProgramLight, "objectColor"), 1, glm::value_ptr(lightData.lightColor));
    glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "view"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(lightData.shaderProgramLight, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));

    // Render the light object
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

void Renderer::drawCleanup()
{
    glDeleteVertexArrays(1, &objData.VAO);
    glDeleteBuffers(1, &objData.VBO);
}