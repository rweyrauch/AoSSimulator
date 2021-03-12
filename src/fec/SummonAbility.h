/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <CommandAbility.h>
#include <string>
#include <vector>

class Roster;

namespace FleshEaterCourt {

    typedef std::vector<std::pair<std::string, int>> SummonedUnitDesc;

    class SummonAbility : public CommandAbility {
    public:
        SummonAbility(Unit *leader, Roster *roster, const std::string &name, const SummonedUnitDesc &summonableUnits);

    protected:

        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }

        SummonedUnitDesc m_summonedUnitDesc;
        Roster *m_roster = nullptr;
    };

}