/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class NeaveBlacktalon : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        NeaveBlacktalon(bool isGeneral);

        ~NeaveBlacktalon() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_boltstormPistol,
                m_whirlwindAxes;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lightning-fast Strikes           Yes
// Tireless Hunter                  Yes
// Nemesis                          Yes
// Windrider                        TODO
//

} // namespace StormcastEternals
