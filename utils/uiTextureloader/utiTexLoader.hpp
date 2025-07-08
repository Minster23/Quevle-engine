#pragma once
#include <core/renderer/shader_h.h>
#include <core/renderer/renderer.hpp>
#include <filesystem>
#include <iostream>
#include <string>


class uiTexLoader{
    public:
    unsigned int loadTex(const std::string& texLocation);
};
