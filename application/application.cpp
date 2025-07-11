#include <core/window/window.hpp>
#include <application/application.hpp>
#include <utils/camera/camera.hpp>

using namespace QuavleEngine;

WindowManager window;

void application::init(){
    window.initWindow();
    window.openGL();
}

void application::tick(){
    window.mainLoop();
}

void application::shutdown(){
    window.cleanup();
}