#include <utils/camera/camera.hpp>

#include "camera.hpp"
std::vector<Camera> cameras;


void initCamera() {
    // * INI UNTUK BUAT MAIN CAMERA
    Camera cam;
    cam.name = "Main Camera";
    cam.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cam.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cam.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cam.yaw = -90.0f;
    cam.pitch = 0.0f;
    cam.lastX = 800.0f / 2.0f;
    cam.lastY = 600.0f / 2.0f;
    cam.fov = 45.0f;
    cam.deltaTime = 0.0f;
    cam.lastFrame = 0.0f;
    cam.firstMouse = true;
    cam.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cam.view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.cameraUp);
    cam.projection = glm::perspective(glm::radians(cam.fov), 800.0f/600.0f, 0.1f, 100.0f);

    cameras.push_back(cam);
}