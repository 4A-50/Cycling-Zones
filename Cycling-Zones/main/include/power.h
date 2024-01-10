#pragma once

#include <array>

#include "imgui.h"
#include "implot.h"

#include "core.h"
#include "wpkg.h"

class Power
{
public:
	Power();
	void PowerGUI(ImGuiWindowFlags window_flags, const ImGuiViewport* main_viewport);

	bool reGenerateWPKGZones = false;

private:
	void GenerateNewPowerZones();

	bool editingPower = false;
	char editPower[128] = "";

	const char* zoneLabels[6] = { "Z1", "Z2", "Z3", "Z4", "Z5", "Z6" };
	std::array<double, 6> zones = { 59,
								    75 - 60,
								    89 - 76,
								    104 - 90,
								    118 - 105,
								    130 - 119 };

	std::array<double, 6> zoneMins = { 0, 60, 76, 90, 105, 119 };
	std::array<double, 6> zoneMaxs = { 59, 75, 89, 104, 118, 130 };
};