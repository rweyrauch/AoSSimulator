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

    class Skullreapers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Skullreapers() = delete;

        ~Skullreapers() override = default;

    protected:

        Skullreapers(SlaughterHost host, int numModels, bool iconBearer);

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        bool battleshockRequired() const override { return false; }

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        Weapon  m_blades{Weapon::Type::Melee, "Gore-slick Blades, Daemonblade, Spinecleavers and Soultearers", 1, 4, 3, 3, 0, 1},
                m_viciousMutation{Weapon::Type::Melee, "Vicious Mutation", 1, 1, 3, 4, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trial of Skulls                  Yes
// Daemonforged Weapons             Yes
// Murderous to the Last            Yes
//

} // namespace Khorne
