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

    class ChaosLordOnKarkadrak : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLordOnKarkadrak();

        ~ChaosLordOnKarkadrak() override = default;

    protected:

        void configure();

        void onCharged() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_battleAxe,
                m_blade,
                m_hornsAndClaws,
                m_tail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brutish Rampage                  Yes
// Daemonbound                      Yes
// Fuelled by Carnage               TODO
// Rune-etched Plating              Yes
// The Knights of Chaos             TODO
//

} // SlavesToDarkness
