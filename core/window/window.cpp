#include <core/window/window.hpp>
#include <core/renderer/renderer.hpp>
#include <string.h>
#include <iostream> // Include iostream here if needed
#include <optional>
#include <set>

using namespace QuavleEngine;
Renderer renderer;

bool isMouseCaptured = false;
double lastPosX = 0.0, lastPosY = 0.0;
float lastYaw = 0.0f, lastPitch = 0.0f;
bool wasRightMousePressed = false;

bool WindowManager::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialize mode after glfwInit()
    mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode) {
        std::cerr << "Failed to get video mode for primary monitor" << std::endl;
        glfwTerminate();
        return false;
    }

    window = glfwCreateWindow(mode->width, mode->height, "Quevle Engine", nullptr, nullptr);

    // glad: load all OpenGL function pointers
    // Note: GLAD initialization should happen *after* creating the window and making the context current.
    // This function should probably just create the window. GLAD init belongs in openGL().
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false; // Return false on failure
    }
    //* nyeting callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glfwWindowHint(GLFW_SAMPLES, 8); // Set before window creation for MSAA x8 (should also be in initWindow)
    glEnable(GL_MULTISAMPLE); // Enable MSAA in OpenGL
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    renderer.init(); // Initialize renderer (includes camera init)
    return true;
}

void WindowManager::mainLoop()
{
    // Initialization moved to renderer.initialize()
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // --- Mouse capture toggle with right mouse button ---
        int rightMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        if (rightMouseState == GLFW_PRESS && !isMouseCaptured) {
            isMouseCaptured = true;
            // Save last mouse position and camera rotation
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastPosX = xpos;
            lastPosY = ypos;
            lastYaw = renderer.cam.yaw;
            lastPitch = renderer.cam.pitch;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (rightMouseState == GLFW_RELEASE && isMouseCaptured) {
            isMouseCaptured = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        // Render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * renderer.cam.deltaTime); // Use renderer's camera deltaTime
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
         renderer.cam.cameraPos += cameraSpeed * renderer.cam.cameraFront; // Update renderer's camera
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
         renderer.cam.cameraPos -= cameraSpeed * renderer.cam.cameraFront; // Update renderer's camera
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
         renderer.cam.cameraPos -= glm::normalize(glm::cross( renderer.cam.cameraFront,  renderer.cam.cameraUp)) * cameraSpeed; // Update renderer's camera
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
         renderer.cam.cameraPos += glm::normalize(glm::cross( renderer.cam.cameraFront,  renderer.cam.cameraUp)) * cameraSpeed; // Update renderer's camera
}


void WindowManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!isMouseCaptured) return; // Do not process mouse movement if not captured

    static bool firstFrame = true;
    if (firstFrame) {
        renderer.cam.lastX = lastPosX;
        renderer.cam.lastY = lastPosY;
        renderer.cam.yaw = lastYaw;
        renderer.cam.pitch = lastPitch;
        firstFrame = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        firstFrame = true;
        return;
    }

    float xoffset = xpos - renderer.cam.lastX;
    float yoffset = renderer.cam.lastY - ypos;
    renderer.cam.lastX = xpos;
    renderer.cam.lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    renderer.cam.yaw   += xoffset;
    renderer.cam.pitch += yoffset;

    if(renderer.cam.pitch > 89.0f)
        renderer.cam.pitch = 89.0f;
    if(renderer.cam.pitch < -89.0f)
        renderer.cam.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(renderer.cam.yaw)) * cos(glm::radians(renderer.cam.pitch));
    direction.y = sin(glm::radians(renderer.cam.pitch));
    direction.z = sin(glm::radians(renderer.cam.yaw)) * cos(glm::radians(renderer.cam.pitch));
    renderer.cam.cameraFront = glm::normalize(direction);
}  

void WindowManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    renderer.cam.fov -= (float)yoffset;
    if (renderer.cam.fov < 1.0f)
        renderer.cam.fov = 1.0f;
    if (renderer.cam.fov > 45.0f)
        renderer.cam.fov = 45.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void WindowManager::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}