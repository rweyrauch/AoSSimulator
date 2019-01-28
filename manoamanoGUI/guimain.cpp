/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "Board.h"
#include "Dice.h"
#include "ManoAMano.h"
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/formhelper.h>
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    int numRounds = 5;
    bool verbose = false;
    int numIterations = 1;
    bool saveMaps = false;

    nanogui::init();

    nanogui::Screen *screen = new nanogui::Screen({256, 512}, "AoS Mano-a-mano GUI", false);
    nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
    auto window = gui->addWindow({0, 0}, "Form helper example");

    gui->addGroup("Settings");
    gui->addVariable("Iterations", numIterations);
    gui->addVariable("Rounds", numRounds);
    gui->addVariable("Verbose", verbose);
    gui->addVariable("Save Maps", saveMaps);

    gui->addGroup("Players");
    gui->addButton("Red", [gui](){ auto redWindow = gui->addWindow(nanogui::Vector2i(0, 0), "Select Red Unit"); redWindow->center(); });
    gui->addButton("Blue", [gui](){ auto blueWindow = gui->addWindow(nanogui::Vector2i(0, 0), "Select Blue Unit"); blueWindow->center(); });

    gui->addGroup("Simulation");
    gui->addButton("Start", [](){ std::cout << "Starting simulation." << std::endl; } );

    screen->setVisible(true);
    screen->performLayout();

    window->center();

    Initialize();

    nanogui::mainloop();

    nanogui::shutdown();
}