#include <core/window/window.hpp>
#include <core/renderer/renderer.hpp>
#include <string.h>
#include <iostream> // Include iostream here if needed
#include <optional>
#include <set>
#include <core/interface/interface.hpp>

using namespace QuavleEngine;
Renderer renderer;
interface Interface;

bool isMouseCaptured = false;
double lastPosX = 0.0, lastPosY = 0.0;
float lastYaw = 0.0f, lastPitch = 0.0f;
bool wasRightMousePressed = false;

WindowManager::WindowManager()
    : m_window(nullptr), m_mode(nullptr), m_FBO(0), m_RBO(0), m_texture_id(0)
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
    if (!m_mode) {
        std::cerr << "Failed to get video mode for primary monitor" << std::endl;
        glfwTerminate();
        return false;
    }

    m_window = glfwCreateWindow(m_mode->width, m_mode->height, "Quevle Engine", nullptr, nullptr);

    // glad: load all OpenGL function pointers
    // Note: GLAD initialization should happen *after* creating the window and making the context current.
    // This function should probably just create the window. GLAD init belongs in openGL().
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
    
    renderer.init();        // Initialize renderer (includes camera init)
    Interface.init(m_window, this);
    return true;
}

void WindowManager::mainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);

        int rightMouseState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);
        if (rightMouseState == GLFW_PRESS && !isMouseCaptured) {
            isMouseCaptured = true;
            // Save last mouse position and camera rotation
            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);
            lastPosX = xpos;
            lastPosY = ypos;
            lastYaw = renderer.cam.yaw;
            lastPitch = renderer.cam.pitch;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (rightMouseState == GLFW_RELEASE && isMouseCaptured) {
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
    if (!isMouseCaptured) return;

    static bool firstFrame = true;
    static float smoothedYaw = 0.0f;
    static float smoothedPitch = 0.0f;

    if (firstFrame) {
        renderer.cam.lastX = lastPosX;
        renderer.cam.lastY = lastPosY;
        renderer.cam.yaw = lastYaw;
        renderer.cam.pitch = lastPitch;
        smoothedYaw = lastYaw;
        smoothedPitch = lastPitch;
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

    // Raw input
    float targetYaw = renderer.cam.yaw + xoffset;
    float targetPitch = renderer.cam.pitch + yoffset;

    // Clamp target pitch
    if (targetPitch > 89.0f)
        targetPitch = 89.0f;
    if (targetPitch < -89.0f)
        targetPitch = -89.0f;

    // Apply smoothing (low-pass filter)
    float smoothingFactor = 0.15f; // smaller = smoother
    smoothedYaw = smoothedYaw + (targetYaw - smoothedYaw) * smoothingFactor;
    smoothedPitch = smoothedPitch + (targetPitch - smoothedPitch) * smoothingFactor;

    renderer.cam.yaw = smoothedYaw;
    renderer.cam.pitch = smoothedPitch;

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