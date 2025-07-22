#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <core/renderer/entity/objectEntity.hpp>
#include <assimp/mesh.h>
#include <utils/fileReader.hpp>
#include <future>
#include <chrono>
#include <thread>

namespace QuavleEngine
{
    class Model
    {
    public:
        void model(const std::string &path, bool gamma = false);
        static std::future<void> result;
        void flushTextureLoadQueue();

    private:
        void setModelData(const aiScene *scene, const std::string &path);
        void loadMaterialAsync(aiMaterial *material, size_t objectIndex);
        std::string locationData;
        
    };
}
