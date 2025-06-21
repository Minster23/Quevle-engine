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

namespace QuavleEngine
{
    class Model{

        public:
        
        Model(std::string const &path, bool gamma);

        private:
        void setModelData(const aiScene *scene, const std::string &path);
    };
}
