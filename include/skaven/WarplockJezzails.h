/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class WarplockJezzails : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WarplockJezzails(int numModels, int points);

        ~WarplockJezzails() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_jezzail,
                m_knives;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Warpstone Snipers                Yes
// Pavise                           Yes
//

} // namespace Skaven
