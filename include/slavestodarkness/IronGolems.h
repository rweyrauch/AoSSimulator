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

    class IronGolems : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        IronGolems(DamnedLegion legion, int numModels);

        IronGolems() = delete;

        ~IronGolems() override = default;

    protected:

        int braveryModifier() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon  m_bolas{Weapon::Type::Missile, "Bolas", 8, 1, 4, 4, 0, 1},
                m_legionWeapons{Weapon::Type::Melee, "Legion Weapons", 1, 1, 4, 4, 0, 1},
                m_legionWeaponsDominar{Weapon::Type::Melee, "Legion Weapons (Dominar)", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Iron Resilience                  Yes
// Signifier                        Yes
//

} // SlavesToDarkness
