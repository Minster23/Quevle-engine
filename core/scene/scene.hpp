#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <core/renderer/entity/objectEntity.hpp>
#include <utils/camera/camera.hpp>

namespace SCENEMANAGER{
    using namespace QuavleEngine;
    class scene{
        public:
        void sceneInit();
        void createScene();
        void updateScene();
        void saveScene();
        void loadScene(char SceneUuid);
        int getSceneIndexByUUID(char SceneUuid) {
            for (size_t i = 0; i < scenes.size(); ++i) {
                if (scenes[i].UUID == SceneUuid) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        struct SceneData {
            std::string name;
            bool isActive = false;
            bool isLoaded = false;
            char UUID;
            std::vector<std::string> objectLoctions;
            std::vector<ObjectEntity::LightData> lights;
            std::vector<ObjectEntity::CubeMap> cubeMaps;
            std::vector<ObjectEntity::Billboard> billboards;
            std::vector<Camera> cameras;
        };
        static std::vector<SceneData> scenes;
        static char selectedSceneUUID;
    };
}