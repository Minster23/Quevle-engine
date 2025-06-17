#include <utils/camera/camera.hpp>
#include <GLFW/glfw3.h>

using namespace QuavleEngine;

void camera::init()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
}

void camera::update()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 camera::getViewMatrix() const
{
    return view;
}