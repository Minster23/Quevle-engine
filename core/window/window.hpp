#pragma once

#include <glad/glad.h>

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
        GLFWwindow *getWindow() const;
        GLuint get_texture_id() const;

        // Window lifecycle
        bool initWindow();
        bool openGL();
        void mainLoop();
        void cleanup();

        static double mousePosX, mousePosY;
        static bool isInteractive;

    private:
        void create_framebuffer();

        // GLFW
        GLFWwindow *m_window;
        const GLFWvidmode *m_mode;

        // Framebuffer
        GLuint m_FBO;
        GLuint m_RBO;
        GLuint m_texture_id;

        // Key states for toggles
        bool m_key_d_pressed;
        bool m_key_s_pressed;
        bool m_key_n_pressed;
        bool m_key_m_pressed;
        bool m_key_r_pressed;

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void refresh();
        void processInput(GLFWwindow *window);
    };
}
