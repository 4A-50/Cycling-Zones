#include "power.h"

Power::Power()
{
    GenerateNewPowerZones();
}

void Power::PowerGUI(ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport)
{
    reGenerateWPKGZones = false;

	//Builds The Core Of The Section Window
	Core::BuildIMGUIWindow(20, 20, 320, 360, window_flags, main_viewport, "Power Zones");

#pragma region Edit FTP
    char tempPowerCharArray[10];
    sprintf_s(tempPowerCharArray, "%u", Core::ftp);

    ImGui::BeginDisabled(!editingPower);
    ImGui::InputTextWithHint("##PowerInput", tempPowerCharArray, editPower, IM_ARRAYSIZE(editPower));
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (!editingPower)
    {
        if (ImGui::Button("Edit##Power"))
            editingPower = true;
    }
    else
    {
        if (ImGui::Button("Save##Power"))
        {
            editingPower = false;

            Core::ftp = std::stof(editPower);
            editPower[0] = 0;

            GenerateNewPowerZones();
            reGenerateWPKGZones = true;
            Core::WriteUserJSON();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##ID"))
        {
            editingPower = false;

            editPower[0] = 0;
        }
    }
#pragma endregion

    ImGui::Separator();

    ImGui::Text("Z1: < %.0fw", zoneMaxs[0]);
    ImGui::Text("Z2: %.0fw -> %.0fw", zoneMins[1], zoneMaxs[1]);
    ImGui::Text("Z3: %.0fw -> %.0fw", zoneMins[2], zoneMaxs[2]);
    ImGui::Text("Z4: %.0fw -> %.0fw", zoneMins[3], zoneMaxs[3]);
    ImGui::Text("Z5: %.0fw -> %.0fw", zoneMins[4], zoneMaxs[4]);
    ImGui::Text("Z6: > %.0fw", zoneMins[5]);

    ImGui::Separator();

    ImPlot::PushColormap(Core::powerColorMap);
    if (ImPlot::BeginPlot("##Power Zones", { -1, 190 * Core::viewportYScaleMultiplier }, ImPlotFlags_NoFrame)) {
        ImPlot::SetupLegend(ImPlotLocation_South, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal);
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoDecorations);

        ImPlot::PlotBarGroups(zoneLabels, zones.data(), 6, 1, 1, 0, ImPlotBarGroupsFlags_Stacked | ImPlotBarGroupsFlags_Horizontal);
        
        ImPlot::EndPlot();
    }
    ImPlot::PopColormap();

	ImGui::End();
}

void Power::GenerateNewPowerZones()
{
    zoneMins = { 0, Core::ftp * 0.60, Core::ftp * 0.76, Core::ftp * 0.90, Core::ftp * 1.05, Core::ftp * 1.19 };

    zoneMaxs = { Core::ftp * 0.59, Core::ftp * 0.75, Core::ftp * 0.89, Core::ftp * 1.04, Core::ftp * 1.18, Core::ftp * 1.30 };

    zones = { zoneMaxs[0],
              zoneMaxs[1] - zoneMins[1],
              zoneMaxs[2] - zoneMins[2],
              zoneMaxs[3] - zoneMins[3],
              zoneMaxs[4] - zoneMins[4],
              zoneMaxs[5] - zoneMins[5] };
}