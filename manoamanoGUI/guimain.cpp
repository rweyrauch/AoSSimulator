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

    nanogui::Screen *screen = new nanogui::Screen(nanogui::Vector2i(500, 700), "AoS Mano-a-mano GUI");
    nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
    nanogui::ref<nanogui::Window> window = gui->addWindow(nanogui::Vector2i(10, 10), "Form helper example");

    gui->addGroup("Settings");
    gui->addVariable("Iterations", numIterations);
    gui->addVariable("Rounds", numRounds);
    gui->addVariable("Verbose", verbose);
    gui->addVariable("Save Maps", saveMaps);

    gui->addGroup("Players");
    gui->addButton("Red", [](){ std::cout << "Red button pressed." << std::endl; });
    gui->addButton("Blue", [](){ std::cout << "Blue button pressed." << std::endl; });

    screen->setVisible(true);
    screen->performLayout();

    window->center();

    nanogui::mainloop();

    nanogui::shutdown();
}