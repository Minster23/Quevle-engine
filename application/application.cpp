#include <core/window/window.hpp>
#include <application/application.hpp>
#include <utils/camera/camera.hpp>

using namespace QuavleEngine;

WindowManager window;

void application::init(){
    std::cout<<"Init app"<< std::endl;
    window.initWindow();
    window.openGL();
    initCamera();
}

void application::tick(){
    std::cout<<"Tick";
    window.mainLoop();
}

void application::shutdown(){
    std::cout<<"Clean";
    window.cleanup();
}