#ifndef QUAVLEENGINE_CAMERA_HPP
#define QUAVLEENGINE_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct Camera {
    std::string name;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 projection;
    glm::mat4 view;
    float deltaTime;
    float lastFrame;
    bool firstMouse;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float fov;
    glm::vec3 worldUp;
};

void initCamera();
void addCamera();

extern std::vector<Camera> cameras;

#endif // QUAVLEENGINE_CAMERA_HPP