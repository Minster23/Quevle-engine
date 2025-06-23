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
        WindowManager();

        // Framebuffer management
        void bind_framebuffer();
        void unbind_framebuffer();
        void rescale_framebuffer(float width, float height);
        GLuint get_texture_id() const;

        // Window lifecycle
        bool initWindow();
        bool openGL();
        void mainLoop();
        void cleanup();

    private:
        void create_framebuffer();

        // GLFW
        GLFWwindow *m_window;
        const GLFWvidmode *m_mode;

        // Framebuffer
        GLuint m_FBO;
        GLuint m_RBO;
        GLuint m_texture_id;

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void processInput(GLFWwindow *window);
    };
}
