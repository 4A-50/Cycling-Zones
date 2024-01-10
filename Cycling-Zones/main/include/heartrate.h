#pragma once

#include <array>

#include "imgui.h"
#include "implot.h"

#include "core.h"

class HeartRate
{
public:
	HeartRate();
	void HeartRateGUI(ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport);

private:
	void GenerateNewHRZones();

	bool editingHR = false;
	char editHR[128] = "";

	const char* zoneLabels[5] = { "Z1", "Z2", "Z3", "Z4", "Z5" };
	std::array<double, 5> zones = { 59,
									75 - 60,
									89 - 76,
									104 - 90,
									118 - 105 };

	std::array<double, 5> zoneMins = { 0, 60, 76, 90, 105 };
	std::array<double, 5> zoneMaxs = { 59, 75, 89, 104, 118 };
};