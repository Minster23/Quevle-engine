#pragma once

#include <iostream>
#include <utils/camera/camera.hpp>


namespace QuavleEngine{
    class saveData{
        public:
        void save();
        void load();
        private:
        void createManifest();
        std::string locationManifest = "projects/engine/quavle.json";

        bool saveObject();
    };
}