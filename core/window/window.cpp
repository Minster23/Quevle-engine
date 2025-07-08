#include <core/window/window.hpp>
#include <core/renderer/renderer.hpp>
#include <string.h>
#include <iostream> // Include iostream here if needed
#include <string>
#include <optional>
#include <set>
#include <core/interface/interface.hpp>
#include <core/scripting/scripting.h>
#include <utils/camera/camera.hpp>

using namespace QuavleEngine;
Scripting::script Scriptings;
Renderer renderer;
interface Interface;

bool isMouseCaptured = true;
double lastPosX = 0.0, lastPosY = 0.0;
float lastYaw = 0.0f, lastPitch = 0.0f;
bool wasRightMousePressed = false;
double QuavleEngine::WindowManager::mousePosX = 0.0;
double QuavleEngine::WindowManager::mousePosY = 0.0;

bool WindowManager::isInteractive = false;

WindowManager::WindowManager()
    : m_window(nullptr), m_mode(nullptr), m_FBO(0), m_RBO(0), m_texture_id(0),
      m_key_d_pressed(false), m_key_s_pressed(false), m_key_n_pressed(false), m_key_m_pressed(false), m_key_r_pressed(false)
{
}

void WindowManager::create_framebuffer()
{
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_mode->width, m_mode->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_mode->width, m_mode->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

// here we bind our framebuffer
void WindowManager::bind_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

// here we unbind our framebuffer
void WindowManager::unbind_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// and we rescale the buffer, so we're able to resize the window
void WindowManager::rescale_framebuffer(float width, float height)
{
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}

GLuint WindowManager::get_texture_id() const
{
    return m_texture_id;
}

bool WindowManager::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Initialize mode after glfwInit()
    m_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!m_mode)
    {
        std::cerr << "Failed to get video mode for primary monitor" << std::endl;
        glfwTerminate();
        return false;
    }

    m_window = glfwCreateWindow(m_mode->width, m_mode->height, "Quevle Engine", nullptr, nullptr);

    if (m_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false; // Return false on failure
    }

    //* nyeting callbacks
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return true; // Return true on success
}

bool WindowManager::openGL()
{
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    create_framebuffer();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE); // Enable MSAA in OpenGL
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Interface.init(m_window, this);
    renderer.init();
    return true;
}

void WindowManager::mainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        float currentFrame = glfwGetTime();
        for (auto& cam : cameras) {
            cam.deltaTime = currentFrame - cam.lastFrame;
            cam.lastFrame = currentFrame;
        }


        if (!play)
        {
            processInput(m_window);
        }

        if (play)
        {
            Scriptings.scripting();
        }

        int rightMouseState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);
        if (rightMouseState == GLFW_PRESS && !isMouseCaptured)
        {
            isMouseCaptured = true;
            // Save last mouse position and camera rotation
            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);
            lastPosX = xpos;
            lastPosY = ypos;
            lastYaw = cameras[0].yaw;
            lastPitch = cameras[0].pitch;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (rightMouseState == GLFW_RELEASE && isMouseCaptured)
        {
            isMouseCaptured = false;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        bind_framebuffer();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.drawCallback();
        unbind_framebuffer();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Interface.interfaceRender();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void WindowManager::cleanup()
{
    Interface.interfaceCleanup();
    renderer.drawCleanup();
    glfwTerminate();
}

void WindowManager::refresh()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void WindowManager::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    Camera& activeCam = cameras[cameraIndex];

    float cameraSpeed = static_cast<float>(2.5 * activeCam.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        activeCam.cameraPos += cameraSpeed * activeCam.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        activeCam.cameraPos -= cameraSpeed * activeCam.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        activeCam.cameraPos -= glm::normalize(glm::cross(activeCam.cameraFront, activeCam.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        activeCam.cameraPos += glm::normalize(glm::cross(activeCam.cameraFront, activeCam.cameraUp)) * cameraSpeed;

    // --- Texture Toggles ---
    bool ctrl_pressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

    // Toggle Diffuse (Ctrl+D)
    if (ctrl_pressed && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !m_key_d_pressed)
    {
        Renderer::Diffuse = !Renderer::Diffuse;
        m_key_d_pressed = true;
        Interface.inputDebug("Info", "Diffuse map toggled " + std::string(Renderer::Diffuse ? "ON" : "OFF"));
        refresh();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
    {
        m_key_d_pressed = false;
    }

    // Toggle Specular (Ctrl+S)
    if (ctrl_pressed && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !m_key_s_pressed)
    {
        Renderer::Specular = !Renderer::Specular;
        m_key_s_pressed = true;
        Interface.inputDebug("Info", "Specular map toggled " + std::string(Renderer::Specular ? "ON" : "OFF"));
        refresh();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
    {
        m_key_s_pressed = false;
    }

    // Toggle Normal (Ctrl+N)
    if (ctrl_pressed && glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !m_key_n_pressed)
    {
        Renderer::Normal = !Renderer::Normal;
        m_key_n_pressed = true;
        Interface.inputDebug("Info", "Normal map toggled " + std::string(Renderer::Normal ? "ON" : "OFF"));
        refresh();
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
    {
        m_key_n_pressed = false;
    }

    // Toggle Metallic (Ctrl+M)
    if (ctrl_pressed && glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !m_key_m_pressed)
    {
        Renderer::Metallic = !Renderer::Metallic;
        m_key_m_pressed = true;
        Interface.inputDebug("Info", "Metallic map toggled " + std::string(Renderer::Metallic ? "ON" : "OFF"));
        refresh();
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
    {
        m_key_m_pressed = false;
    }

    // Toggle Roughness (Ctrl+R)
    if (ctrl_pressed && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !m_key_r_pressed)
    {
        Renderer::Roughness = !Renderer::Roughness;
        m_key_r_pressed = true;
        Interface.inputDebug("Info", "Roughness map toggled " + std::string(Renderer::Roughness ? "ON" : "OFF"));
        refresh();
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        m_key_r_pressed = false;
    }

    activeCam.view = glm::lookAt(activeCam.cameraPos, activeCam.cameraPos + activeCam.cameraFront, activeCam.cameraUp);
    activeCam.projection = glm::perspective(glm::radians(activeCam.fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

void WindowManager::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!isMouseCaptured)
        return;

    static bool firstFrame = true;
    static float smoothedYaw = 0.0f;
    static float smoothedPitch = 0.0f;

    if (firstFrame)
    {
        cameras[0].lastX = lastPosX;
        cameras[0].lastY = lastPosY;
        cameras[0].yaw = lastYaw;
        cameras[0].pitch = lastPitch;
        smoothedYaw = lastYaw;
        smoothedPitch = lastPitch;
        firstFrame = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        firstFrame = true;
        return;
    }

    float xoffset = xpos - cameras[0].lastX;
    float yoffset = cameras[0].lastY - ypos;
    cameras[0].lastX = xpos;
    cameras[0].lastY = ypos;

    float sensitivity = 0.9f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Raw input
    float targetYaw = cameras[0].yaw + xoffset;
    float targetPitch = cameras[0].pitch + yoffset;

    // Clamp target pitch
    if (targetPitch > 89.0f)
        targetPitch = 89.0f;
    if (targetPitch < -89.0f)
        targetPitch = -89.0f;

    // Apply smoothing (low-pass filter)
    float smoothingFactor = 0.15f; // smaller = smoother
    smoothedYaw = smoothedYaw + (targetYaw - smoothedYaw) * smoothingFactor;
    smoothedPitch = smoothedPitch + (targetPitch - smoothedPitch) * smoothingFactor;

    cameras[0].yaw = smoothedYaw;
    cameras[0].pitch = smoothedPitch;

    mousePosX = xpos;
    mousePosY = ypos;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cameras[0].yaw)) * cos(glm::radians(cameras[0].pitch));
    direction.y = sin(glm::radians(cameras[0].pitch));
    direction.z = sin(glm::radians(cameras[0].yaw)) * cos(glm::radians(cameras[0].pitch));
    cameras[0].cameraFront = glm::normalize(direction);

    cameras[0].view = glm::lookAt(cameras[0].cameraPos, cameras[0].cameraPos + cameras[0].cameraFront, cameras[0].cameraUp);
    cameras[0].projection = glm::perspective(glm::radians(cameras[0].fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

void WindowManager::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    cameras[0].fov -= (float)yoffset;
    if (cameras[0].fov < 1.0f)
        cameras[0].fov = 1.0f;
    if (cameras[0].fov > 45.0f)
        cameras[0].fov = 45.0f;

    cameras[0].view = glm::lookAt(cameras[0].cameraPos, cameras[0].cameraPos + cameras[0].cameraFront, cameras[0].cameraUp);
    cameras[0].projection = glm::perspective(glm::radians(cameras[0].fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void WindowManager::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}