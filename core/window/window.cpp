#include <core/window/window.hpp>
#include <core/renderer/renderer.hpp>
#include <string.h>
#include <iostream> // Include iostream here if needed
#include <optional>
#include <set>

using namespace QuavleEngine;
Renderer renderer;



bool WindowManager::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Quevle Engine", nullptr, nullptr);

    // glad: load all OpenGL function pointers
    // Note: GLAD initialization should happen *after* creating the window and making the context current.
    // This function should probably just create the window. GLAD init belongs in openGL().
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false; // Return false on failure
    }

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return true; // Return true on success
}

bool WindowManager::openGL()
{
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    renderer.shaderLoader();
    return true;
}

void WindowManager::mainLoop()
{
    renderer.shaderLink();
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // Render
        // ------
        glClearColor(1.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawCallback();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void WindowManager::cleanup()
{
    renderer.drawCleanup();
    glfwTerminate();
}

void WindowManager::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void WindowManager::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}