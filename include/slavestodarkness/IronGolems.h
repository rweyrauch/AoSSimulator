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

        IronGolems();

        ~IronGolems() override = default;

        bool configure(int numModels);

    protected:

        int braveryModifier() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_bolas,
                m_legionWeapons,
                m_legionWeaponsDominar;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Iron Resilience                  Yes
// Signifier                        Yes
//

} // SlavesToDarkness
