/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class SpireTyrants : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SpireTyrants(DamnedLegion legion, int numModels, int points);

        ~SpireTyrants() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_gladiatorialWeapons{Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 1},
                m_gladiatorialWeaponsChampion{Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1},
                m_gladiatorialWeaponsHeadclaimer{Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 2},
                m_gladiatorialWeaponsDestroyer{Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pit Fighters                     Yes
//

} // SlavesToDarkness

