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

    //* DOENST USE
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model = glm::mat4(1.0f);
    float deltaTime;
    float lastFrame;
    bool firstMouse;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float fov;
    glm::vec3 worldUp;
    bool isSelected = false;
};

void initCamera();
void addCamera();

extern std::vector<Camera> cameras;

#endif // QUAVLEENGINE_CAMERA_HPP