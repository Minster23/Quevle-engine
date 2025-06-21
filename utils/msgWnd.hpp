#pragma once

#include <string>

namespace QuavleEngine {
    void windowMessage(const std::string& title, const std::string& message);
    void windowMessageWithProgress(const std::string& title, const std::string& message, float progress);
    void windowMessageWithProgressReal(const std::string& title, const std::string& message, float progress); // ✅ Tambahan
}

