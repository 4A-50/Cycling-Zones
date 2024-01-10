#include "wpkg.h"

WPKG::WPKG()
{
    GenerateNewWPKGZones();
}

void WPKG::WPKGGUI(ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport, bool _reGenerateWPKGZones)
{
    if (_reGenerateWPKGZones)
        GenerateNewWPKGZones();

	//Builds The Core Of The Section Window
	Core::BuildIMGUIWindow(20, 400, 660, 180, window_flags, main_viewport, "W/KG Zones");

#pragma region Edit Weight
    char tempPowerCharArray[10];
    sprintf_s(tempPowerCharArray, "%0.1f", Core::weight);

    ImGui::BeginDisabled(!editingWeight);
    ImGui::InputTextWithHint("##PowerInput", tempPowerCharArray, editWeight, IM_ARRAYSIZE(editWeight));
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (!editingWeight)
    {
        if (ImGui::Button("Edit##Power"))
            editingWeight = true;
    }
    else
    {
        if (ImGui::Button("Save##Power"))
        {
            editingWeight = false;

            Core::weight = std::stof(editWeight);
            editWeight[0] = 0;

            GenerateNewWPKGZones();
            Core::WriteUserJSON();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##ID"))
        {
            editingWeight = false;

            editWeight[0] = 0;
        }
    }
#pragma endregion

    ImGui::Separator();

    if (ImGui::BeginTable("##WPKGTable", 3))
    {
        ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
                ImGui::Text("Z1: < %.1fw/kg", zoneMaxs[0]);
            ImGui::TableSetColumnIndex(1);
                ImGui::Text("Z2: %.1fw -> %.1fw/kg", zoneMins[1], zoneMaxs[1]);
            ImGui::TableSetColumnIndex(2);
                ImGui::Text("Z3: %.1fw -> %.1fw/kg", zoneMins[2], zoneMaxs[2]);
        ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
                ImGui::Text("Z4: %.1fw -> %.1fw/kg", zoneMins[3], zoneMaxs[3]);
            ImGui::TableSetColumnIndex(1);
                ImGui::Text("Z5: %.1fw -> %.1fw/kg", zoneMins[4], zoneMaxs[4]);
            ImGui::TableSetColumnIndex(2);
                ImGui::Text("Z6: > %.1fw", zoneMins[5]);

        ImGui::EndTable();
    }

    ImGui::Separator();

    ImPlot::PushColormap(Core::powerColorMap);
    if (ImPlot::BeginPlot("##Power Zones", { -1, 75 * Core::viewportYScaleMultiplier }, ImPlotFlags_NoFrame | ImPlotFlags_NoLegend)) {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoDecorations);

        ImPlot::PlotBarGroups(zoneLabels, zones.data(), 6, 1, 1, 0, ImPlotBarGroupsFlags_Stacked | ImPlotBarGroupsFlags_Horizontal);

        ImPlot::EndPlot();
    }
    ImPlot::PopColormap();

    ImGui::End();
}

void WPKG::GenerateNewWPKGZones()
{
    zoneMins = { 0,
                 (Core::ftp * 0.60) / Core::weight,
                 (Core::ftp * 0.76) / Core::weight,
                 (Core::ftp * 0.90) / Core::weight,
                 (Core::ftp * 1.05) / Core::weight,
                 (Core::ftp * 1.19) / Core::weight };

    zoneMaxs = { (Core::ftp * 0.59) / Core::weight,
                 (Core::ftp * 0.75) / Core::weight,
                 (Core::ftp * 0.89) / Core::weight,
                 (Core::ftp * 1.04) / Core::weight,
                 (Core::ftp * 1.18) / Core::weight,
                 (Core::ftp * 1.30) / Core::weight };

    zones = { zoneMaxs[0],
              zoneMaxs[1] - zoneMins[1],
              zoneMaxs[2] - zoneMins[2],
              zoneMaxs[3] - zoneMins[3],
              zoneMaxs[4] - zoneMins[4],
              zoneMaxs[5] - zoneMins[5] };
}