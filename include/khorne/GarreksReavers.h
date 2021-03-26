/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class GarreksReavers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GarreksReavers();

        ~GarreksReavers() override = default;

    protected:

        void configure();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_garreksAxe,
                m_karusAxe,
                m_saeksAxe,
                m_blades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gorebeard                        Yes
// Frenzied Devotion                Yes
// Reaver Blades                    Yes
// Grisly Trophies                  TODO
//

} // namespace Khorne
