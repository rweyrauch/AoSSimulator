/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <WarhammerSim.h>

std::string PhaseToString(Phase phase)
{
    switch (phase)
    {
        case Phase::Initiative:
            return "Initiative";
        case Phase::Hero:
            return "Hero";
        case Phase::Movement:
            return "Movement";
        case Phase::Shooting:
            return "Shooting";
        case Phase::Charge:
            return "Charge";
        case Phase::Combat:
            return "Combat";
        case Phase::Battleshock:
            return "Battleshock";
    }
}

std::string PlayerIdToString(PlayerId id)
{
    switch (id)
    {
        case PlayerId::None:
            return "None";
        case PlayerId::Red:
            return "Red";
        case PlayerId::Blue:
            return "Blue";
    }
}
