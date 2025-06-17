#ifndef QUAVLEENGINE_CAMERA_HPP
#define QUAVLEENGINE_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace QuavleEngine
{

    class camera
    {
    public:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        bool firstMouse = true;
        float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch = 0.0f;
        float lastX = 800.0f / 2.0;
        float lastY = 600.0 / 2.0;
        float fov = 45.0f;

        float deltaTime; // Time between current frame and last frame
        float lastFrame; // Time of last frame

        void init();
        void update();
        glm::mat4 getViewMatrix() const;

    private:
        glm::mat4 view;
    };
}

#endif // QUAVLEENGINE_CAMERA_HPP