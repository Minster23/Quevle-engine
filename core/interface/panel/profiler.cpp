#include <core/interface/interface.hpp>
#include <deque>
#include <cstdlib> // for rand()
#include <ctime>

using namespace QuavleEngine;

#include <implot.h>
#include <deque>
#include <cstdlib> // for rand()
#include <ctime>

void interface::profiler() {
    ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    static std::deque<float> fpsHistory;
    static std::deque<float> cpuHistory;
    static std::deque<float> gpuHistory;
    const int MAX_SAMPLES = 200;

    // === Simulate data ===
    float fps = ImGui::GetIO().Framerate;
    float cpuUsage = 40.0f + std::rand() % 60;  // Simulated 40-100%
    float gpuUsage = 30.0f + std::rand() % 70;  // Simulated 30-100%

    fpsHistory.push_back(fps);
    cpuHistory.push_back(cpuUsage);
    gpuHistory.push_back(gpuUsage);

    if (fpsHistory.size() > MAX_SAMPLES) fpsHistory.pop_front();
    if (cpuHistory.size() > MAX_SAMPLES) cpuHistory.pop_front();
    if (gpuHistory.size() > MAX_SAMPLES) gpuHistory.pop_front();

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / fps);
    ImGui::Text("CPU Usage: %.1f%%", cpuUsage);
    ImGui::Text("GPU Usage: %.1f%%", gpuUsage);

    // Helper to convert deque to std::vector<float>
    auto toVec = [](const std::deque<float>& dq) -> std::vector<float> {
        return std::vector<float>(dq.begin(), dq.end());
    };

    std::vector<float> xValues(fpsHistory.size());
    for (int i = 0; i < xValues.size(); ++i) xValues[i] = (float)i;

    // === FPS Plot ===
    if (ImPlot::BeginPlot("FPS", ImVec2(-1, 150))) {
        auto y = toVec(fpsHistory);
        ImPlot::SetupAxes("Frame", "FPS", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("FPS", xValues.data(), y.data(), (int)y.size());
        ImPlot::EndPlot();
    }

    // === CPU Usage Plot ===
    if (ImPlot::BeginPlot("CPU Usage", ImVec2(-1, 150))) {
        auto y = toVec(cpuHistory);
        ImPlot::SetupAxes("Frame", "% CPU", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("CPU", xValues.data(), y.data(), (int)y.size());
        ImPlot::EndPlot();
    }

    // === GPU Usage Plot ===
    if (ImPlot::BeginPlot("GPU Usage", ImVec2(-1, 150))) {
        auto y = toVec(gpuHistory);
        ImPlot::SetupAxes("Frame", "% GPU", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("GPU", xValues.data(), y.data(), (int)y.size());
        ImPlot::EndPlot();
    }

    ImGui::End();
}
