#include <utils/fileReader.hpp>

std::string readFile(const std::string &path)
{
    DEBUG_PRINT("Renderer::readFile() called with path: " + path);
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}