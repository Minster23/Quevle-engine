#include <core/scene/scene.hpp>
#include <core/model/UUID.hpp>

using namespace SCENEMANAGER;
using namespace QuavleEngine;

std::vector<SCENEMANAGER::scene::SceneData> SCENEMANAGER::scene::scenes;
static int sceneCounter = 0;
char SCENEMANAGER::scene::selectedSceneUUID = 0;

void SCENEMANAGER::scene::sceneInit(){
    if(scenes.empty()) {
        createScene();
        loadScene(scenes[0].UUID);
    }
}

void SCENEMANAGER::scene::createScene(){
    SceneData newScene;
    newScene.name = "New Scene " + std::to_string(sceneCounter++);
    newScene.isActive = true;
    newScene.isLoaded = false;
    newScene.UUID = UUID::generate_uuid()[234];
    newScene.objectLoctions = {};
    newScene.lights = {};
    newScene.cubeMaps = {};
    newScene.billboards = {};
    newScene.cameras = {};

    scenes.push_back(newScene);
}

void SCENEMANAGER::scene::updateScene(){
    
}

void SCENEMANAGER::scene::saveScene(){
    
}

void SCENEMANAGER::scene::loadScene(char SceneUuid) {
    selectedSceneUUID = SceneUuid;
    for (auto& scene : scenes) {
        if (scene.UUID == SceneUuid) {
            scene.isLoaded = true;
            std::cout << "Scene " << scene.name << " loaded successfully." << std::endl;
            return;
        }
    }
}
