#pragma once

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <map>

namespace QuavleEngine
{
    class WindowManager
    {
    public:
        bool initWindow();
        bool openGL();
        void mainLoop();
        void cleanup();

    private:
        // GLFW
        GLFWwindow *window;
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        void processInput(GLFWwindow *window);

        // settings
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;
    };
}
