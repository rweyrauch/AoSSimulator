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

    class Skulltaker : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Skulltaker() = delete;

        ~Skulltaker() override = default;

    protected:

        Skulltaker(SlaughterHost host, bool isGenera);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_slayerSword{Weapon::Type::Melee, "The Slayer Sword", 1, 3, 3, 3, -1, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Skulls                  Yes
// Decapitating Strike              Yes
// Heroes' Bane                     Yes
// Heads Must Roll                  Yes
//

} // namespace Khorne
