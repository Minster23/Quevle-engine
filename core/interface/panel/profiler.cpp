#include <core/interface/interface.hpp>
#include <deque>
#include <cstdlib> // for rand()
#include <ctime>

using namespace QuavleEngine;

#include <implot.h>
#include <deque>
#include <cstdlib> // for rand()
#include <ctime>

static glm::vec3 colorSource = glm::vec3(1.0f, 0.0f, 0.0f); // red
static glm::vec3 colorTarget = glm::vec3(0.0f, 0.0f, 1.0f); // blue

void ShowColorDragDropExample()
{
    ImGui::Begin("Color Drag & Drop");

    // --- Source Widget ---
    ImGui::Text("Drag Source:");
    ImGui::ColorEdit3("##SourceColor", (float*)&colorSource, ImGuiColorEditFlags_NoInputs);
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("COLOR_VEC3", &colorSource, sizeof(glm::vec3));
        ImGui::Text("Drop Color");
        ImGui::ColorButton("##dragpreview", ImVec4(colorSource.r, colorSource.g, colorSource.b, 1.0f));
        ImGui::EndDragDropSource();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Target Widget ---
    ImGui::Text("Drop Target:");
    ImGui::ColorEdit3("##TargetColor", (float*)&colorTarget, ImGuiColorEditFlags_NoInputs);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COLOR_VEC3"))
        {
            IM_ASSERT(payload->DataSize == sizeof(glm::vec3));
            const glm::vec3* droppedColor = static_cast<const glm::vec3*>(payload->Data);
            colorTarget = *droppedColor;
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}


void interface::profiler()
{
    ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    static std::deque<float> fpsHistory;
    static std::deque<float> cpuHistory;
    static std::deque<float> gpuHistory;
    const int MAX_SAMPLES = 200;

    // Simulated data (robotic/static feel by smoothing and occasional flat zones)
    float fps = ImGui::GetIO().Framerate;
    float cpuUsage = 40.0f + std::rand() % 60;
    float gpuUsage = 30.0f + std::rand() % 70;

    // Add robotic flat zones
    static int holdFrames = 0;
    static float lastFlatCPU = cpuUsage;
    static float lastFlatGPU = gpuUsage;

    if (holdFrames == 0 && (std::rand() % 20 == 0))
    {
        holdFrames = 10 + std::rand() % 20;
        lastFlatCPU = cpuUsage;
        lastFlatGPU = gpuUsage;
    }

    if (holdFrames > 0)
    {
        cpuUsage = lastFlatCPU;
        gpuUsage = lastFlatGPU;
        holdFrames--;
    }

    // Light smoothing
    if (!cpuHistory.empty())
        cpuUsage = (cpuUsage + cpuHistory.back()) * 0.5f;
    if (!gpuHistory.empty())
        gpuUsage = (gpuUsage + gpuHistory.back()) * 0.5f;

    fpsHistory.push_back(fps);
    cpuHistory.push_back(cpuUsage);
    gpuHistory.push_back(gpuUsage);

    if (fpsHistory.size() > MAX_SAMPLES)
        fpsHistory.pop_front();
    if (cpuHistory.size() > MAX_SAMPLES)
        cpuHistory.pop_front();
    if (gpuHistory.size() > MAX_SAMPLES)
        gpuHistory.pop_front();

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / fps);
    ImGui::Text("CPU Usage: %.1f%%", cpuUsage);
    ImGui::Text("GPU Usage: %.1f%%", gpuUsage);

    auto toVec = [](const std::deque<float> &dq) -> std::vector<float>
    {
        return std::vector<float>(dq.begin(), dq.end());
    };

    std::vector<float> xValues(fpsHistory.size());
    for (int i = 0; i < xValues.size(); ++i)
        xValues[i] = (float)i;

    ImPlot::PushStyleColor(ImPlotCol_FrameBg, IM_COL32(10, 10, 10, 255));
    ImPlot::PushStyleColor(ImPlotCol_Line, IM_COL32(255, 0, 0, 255)); // red color
    ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2.0f);
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0.0f);

    auto plotHeartbeat = [](const char *title, const char *label, const std::vector<float> &x, const std::vector<float> &y, const char *unit)
    {
        ImGui::Text("%s", label); // Left-side label before plot
        if (ImPlot::BeginPlot(title, ImVec2(-1, 120), ImPlotFlags_CanvasOnly | ImPlotFlags_NoTitle | ImPlotFlags_NoLegend))
        {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
            ImPlot::SetupAxisLimits(ImAxis_X1, x.front(), x.back(), ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 110, ImGuiCond_Always);

            ImPlot::PlotLine(title, x.data(), y.data(), (int)y.size());

            // Annotate latest value
            if (!x.empty() && !y.empty())
            {
                float xLast = x.back();
                float yLast = y.back();

                char buf[32];
                snprintf(buf, sizeof(buf), "%.1f%s", yLast, unit);
                ImPlot::PlotText(buf, xLast, yLast, ImVec2(8, -12));
            }

            ImPlot::EndPlot();
        }
    };

    plotHeartbeat("FPS Monitor", "FPS", xValues, toVec(fpsHistory), "");
    plotHeartbeat("CPU Monitor", "CPU", xValues, toVec(cpuHistory), "%%");
    plotHeartbeat("GPU Monitor", "GPU", xValues, toVec(gpuHistory), "%%");

    ImPlot::PopStyleVar(2);
    ImPlot::PopStyleColor(2);

    ImGui::End();
}
