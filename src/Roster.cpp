/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Roster.h>

void Roster::doBattleshockPhase()
{
    for (auto u : m_units)
    {
        u.battleshock(0);
    }
}
