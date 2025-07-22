#pragma once

#include <iostream>
#include <string>

namespace Scripting
{
    class script{
        public:
        void scripting();
        void compileCode();
        void updateInput();
        void newScript(std::string scriptName, int index);
    };
}