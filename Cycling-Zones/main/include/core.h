#pragma once

#include <string>
#include <iterator>
#include <fstream>

#include "imgui.h"
#include "json.hpp"

#define INIT_VIDEO_SIZE_X 700
#define INIT_VIDEO_SIZE_Y 600

using json = nlohmann::json;

class Core
{
public:
    static void BuildIMGUIWindow(int xPos, int yPos, int width, int height, ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport, const char title[]);
    static void GenerateViewportScaleMultiplier(const ImGuiViewport* main_viewport);
    static void CreatePlotColorMaps();

    static void ReadUserJSON();
    static void WriteUserJSON();

    static inline float viewportXScaleMultiplier = 1;
    static inline float viewportYScaleMultiplier = 1;

    static inline int ftp = 200;
    static inline int maxHR = 200;
    static inline float weight = 75.0f;

    static inline ImPlotColormap powerColorMap = -1;
    static inline ImPlotColormap hrColorMap = -1;
};

/// <summary>
/// Builds An IMGUI Window
/// </summary>
/// <param name="xPos">Window X Position</param>
/// <param name="yPos">Window Y Position</param>
/// <param name="width">Window Width</param>
/// <param name="height">Window Height</param>
/// <param name="window_flags">Window Flags</param>
/// <param name="main_viewport">Main Viewport</param>
/// <param name="title">Window Title</param>
inline void Core::BuildIMGUIWindow(int xPos, int yPos, int width, int height, ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport, const char title[])
{
    //Set Windows Pos & Size
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (xPos * viewportXScaleMultiplier), main_viewport->WorkPos.y + (yPos * viewportYScaleMultiplier)), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((width * viewportXScaleMultiplier), (height * viewportYScaleMultiplier)), ImGuiCond_Always);

    if (!ImGui::Begin(title, NULL, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
}

/// <summary>
/// Generates The Current Frames Viewport Scale Multiplier For Everything To Scale With
/// </summary>
/// <param name="main_viewport">Main Viewport</param>
inline void Core::GenerateViewportScaleMultiplier(const ImGuiViewport* main_viewport)
{
    //Calculates A Scale Multiplier To Make Sure The Windows Look The Same At Any Size
    viewportXScaleMultiplier = main_viewport->WorkSize.x / INIT_VIDEO_SIZE_X;
    viewportYScaleMultiplier = main_viewport->WorkSize.y / INIT_VIDEO_SIZE_Y;
}

/// <summary>
/// Generates The Power & HR Zone Colour Maps
/// </summary>
inline void Core::CreatePlotColorMaps()
{
    if (powerColorMap == -1) {
        ImU32 powerColorMapData[6] = { IM_COL32(115, 115, 115, 255),
                                                    IM_COL32(0, 127, 255, 255),
                                                    IM_COL32(12, 187, 65, 255),
                                                    IM_COL32(255, 196, 0, 255),
                                                    IM_COL32(255, 77, 29, 255),
                                                    IM_COL32(255, 1, 0, 255) };
        powerColorMap = ImPlot::AddColormap("Power Zones", powerColorMapData, 6);
    }

    if (hrColorMap == -1) {
        ImU32 hrColorMapData[5] = { IM_COL32(231, 217, 218, 255),
                                    IM_COL32(229, 193, 193, 255),
                                    IM_COL32(217, 167, 168, 255),
                                    IM_COL32(251, 0, 23, 255),
                                    IM_COL32(180, 3, 18, 255)};
        hrColorMap = ImPlot::AddColormap("HR Zones", hrColorMapData, 5);
    }
}

/// <summary>
/// Reads A `user.json` File To Get The Current Users Stats
/// </summary>
inline void Core::ReadUserJSON()
{
    if (!std::filesystem::exists("user.json"))
    {
        WriteUserJSON();
    }

    std::ifstream f("user.json");
    json savedUserJson = json::parse(f);

    ftp = savedUserJson["ftp"];
    maxHR = savedUserJson["maxHR"];
    weight = savedUserJson["weight"];

    f.close();
}

/// <summary>
/// Saves The Current Users Stats To A `user.json` File
/// </summary>
inline void Core::WriteUserJSON()
{
    json currentUserJson;

    currentUserJson["ftp"] = ftp;
    currentUserJson["maxHR"] = maxHR;
    currentUserJson["weight"] = weight;

    std::ofstream file("user.json");
    file << currentUserJson;
    file.flush();
}