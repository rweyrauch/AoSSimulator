/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class SplinteredFang : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SplinteredFang(DamnedLegion legion, int numModels, int points);

        SplinteredFang() = delete;

        ~SplinteredFang() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_poisonedWeapons{Weapon::Type::Melee, "Poisoned Weapons", 1, 1, 4, 4, 0, 1},
                m_poisonedWeaponsLeader{Weapon::Type::Melee, "Poisoned Weapons (Trueblood)", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Snake Charmer                    Yes
// One Cut, One Kill                Yes
//

} // SlavesToDarkness
