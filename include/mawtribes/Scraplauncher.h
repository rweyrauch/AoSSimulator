/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class GnoblarScraplauncher : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        GnoblarScraplauncher() = delete;

        ~GnoblarScraplauncher() override = default;

    protected:

        GnoblarScraplauncher(Mawtribe tribe);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_scrap{Weapon::Type::Missile, "Piles of Old Scrap", 36, 3, 3, 4, 0, RAND_D3},
                m_scrapperWeapons{Weapon::Type::Melee, "Gnoblar Scrappers' Weapons", 1, 7, 5, 5, 0, 1},
                m_horns{Weapon::Type::Melee, "Rhinox's Sharp Horns", 1, 1, 4, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Rain of Scrap             Yes
// Rhinox Charge                    Yes
//

} // namespace OgorMawtribes

