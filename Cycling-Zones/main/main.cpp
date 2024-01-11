#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"
#include "SFML/Graphics.hpp"

#include "core.h"
#include "power.h"
#include "heartrate.h"
#include "wpkg.h"

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(INIT_VIDEO_SIZE_X, INIT_VIDEO_SIZE_Y), "Cycling Zones", sf::Style::Default);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;

    //Set The Window Icon
    sf::Image windowIcon;
    if (windowIcon.loadFromFile("Cycling-Zones-Icon@0.5x.png"))
        window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());

    //Load Any Saved User Data
    Core::ReadUserJSON();

    //Init The Window Classes
    Power power = Power();
    HeartRate heartRate = HeartRate();
    WPKG wpkg = WPKG();

    //ImGUI Window Flags
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoBringToFrontOnFocus;

    //ImGui & ImPlot Contexts
    ImGui::CreateContext();
    ImPlot::CreateContext();

    Core::CreatePlotColorMaps();

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        Core::GenerateViewportScaleMultiplier(main_viewport);

        //--- Render The UI
        power.PowerGUI(window_flags, main_viewport);

        heartRate.HeartRateGUI(window_flags, main_viewport);

        wpkg.WPKGGUI(window_flags, main_viewport, power.reGenerateWPKGZones);
        //---

        window.clear(sf::Color(17, 17, 17));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}