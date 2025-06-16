#pragma once
#include <core/window/window.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace QuavleEngine
{

    class Renderer
    {
    public:
        void shaderLoader();
        void shaderLink();
        void drawCallback();
        void drawCleanup();
        void loadTexture();

    private:
        int success;
        char infoLog[512];
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;

        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;

        int width, height, nrChannels;
        unsigned int texture1, texture2;

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "layout (location = 1) in vec3 aColor;\n"
                                         "layout (location = 2) in vec2 aTexCoord;\n"
                                         "\n"
                                         "out vec3 ourColor;\n"
                                         "out vec2 TexCoord;\n"
                                         "\n"
                                         "uniform mat4 model;\n"
                                         "uniform mat4 view;\n"
                                         "uniform mat4 projection;\n"
                                         "\n"
                                         "void main()\n"
                                         "{\n"
                                         "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                         "    ourColor = aColor;\n"
                                         "    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n"
                                         "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "in vec3 ourColor;\n"
                                           "in vec2 TexCoord;\n"
                                           "uniform sampler2D texture1;\n"
                                           "void main()\n"
                                           "{\n"
                                           "    vec4 texColor = texture(texture1, TexCoord);\n"
                                           "    vec4 blendColor = vec4(1.0, 1.0, 1.0, 1.0); // white (or use another color)\n"
                                           "    FragColor = mix(texColor, blendColor, 0.2) * vec4(ourColor, 1.0);\n"
                                           "}\n\0";
    };

}