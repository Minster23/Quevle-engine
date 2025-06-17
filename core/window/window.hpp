#pragma once

#include <glad.h> // Include GLAD first

#define GLFW_INCLUDE_NONE // Tell GLFW not to include the system GL header
#include <GLFW/glfw3.h>   // Include GLFW after GLAD and defining GLFW_INCLUDE_NONE

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <map>

namespace QuavleEngine
{
    class WindowManager
    {
    public:
        const GLFWvidmode* mode;
        // initWindow should return bool, not int
        bool initWindow();
        bool openGL();
        void mainLoop();
        void cleanup();

    private:
        // GLFW
        GLFWwindow *window;

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void processInput(GLFWwindow *window);
    };
}
