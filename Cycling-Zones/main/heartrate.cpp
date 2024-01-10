#include "heartrate.h"

HeartRate::HeartRate()
{
    GenerateNewHRZones();
}

void HeartRate::HeartRateGUI(ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport)
{
	//Builds The Core Of The Section Window
	Core::BuildIMGUIWindow(360, 20, 320, 360, window_flags, main_viewport, "Heart Rate Zones");

#pragma region Edit HR
    char tempPowerCharArray[10];
    sprintf_s(tempPowerCharArray, "%u", Core::maxHR);

    ImGui::BeginDisabled(!editingHR);
    ImGui::InputTextWithHint("##PowerInput", tempPowerCharArray, editHR, IM_ARRAYSIZE(editHR));
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (!editingHR)
    {
        if (ImGui::Button("Edit##Power"))
            editingHR = true;
    }
    else
    {
        if (ImGui::Button("Save##Power"))
        {
            editingHR = false;

            Core::maxHR = std::stof(editHR);
            editHR[0] = 0;

            GenerateNewHRZones();
            Core::WriteUserJSON();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##ID"))
        {
            editingHR = false;

            editHR[0] = 0;
        }
    }
#pragma endregion

    ImGui::Separator();

    ImGui::Text("Z1: < %.0fw", zoneMaxs[0]);
    ImGui::Text("Z2: %.0fw -> %.0fw", zoneMins[1], zoneMaxs[1]);
    ImGui::Text("Z3: %.0fw -> %.0fw", zoneMins[2], zoneMaxs[2]);
    ImGui::Text("Z4: %.0fw -> %.0fw", zoneMins[3], zoneMaxs[3]);
    ImGui::Text("Z5: > %.0fw", zoneMins[4]);
    ImGui::Text("");

    ImGui::Separator();

    ImPlot::PushColormap(Core::hrColorMap);
    if (ImPlot::BeginPlot("##HR Zones", { -1, 190 * Core::viewportYScaleMultiplier }, ImPlotFlags_NoFrame)) {
        ImPlot::SetupLegend(ImPlotLocation_South, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal);
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoDecorations);

        ImPlot::PlotBarGroups(zoneLabels, zones.data(), 5, 1, 1, 0, ImPlotBarGroupsFlags_Stacked | ImPlotBarGroupsFlags_Horizontal);

        ImPlot::EndPlot();
    }
    ImPlot::PopColormap();

    ImGui::End();
}

void HeartRate::GenerateNewHRZones()
{
    zoneMins = { 0, Core::maxHR * 0.66, Core::maxHR * 0.81, Core::maxHR * 0.86, Core::maxHR * 0.93 };

    zoneMaxs = { Core::maxHR * 0.65, Core::maxHR * 0.80, Core::maxHR * 0.85, Core::maxHR * 0.92, Core::maxHR * 1.00 };

    zones = { zoneMaxs[0],
              zoneMaxs[1] - zoneMins[1],
              zoneMaxs[2] - zoneMins[2],
              zoneMaxs[3] - zoneMins[3],
              zoneMaxs[4] - zoneMins[4] };
}