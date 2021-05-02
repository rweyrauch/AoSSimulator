/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class StormsiresCursebreakers : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        StormsiresCursebreakers(Lore lore);

        ~StormsiresCursebreakers() override = default;

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        int generateMortalWounds(const Unit *unit) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_tempestBladeAndStave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blessed Banishment               Yes
// Celestial Lightning Arc          Yes
// Empower                          Yes
//

} // namespace StormcastEternals
