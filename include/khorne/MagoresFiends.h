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

    class MagoresFiends : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~MagoresFiends() override = default;

    protected:

        MagoresFiends();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Rerolls chargeRerolls() const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        Weapon  m_bellyMaw{Weapon::Type::Melee, "Magore's Belly Maw", 1, 1, 4, 3, -1, RAND_D3},
                m_daemonicAxe{Weapon::Type::Melee, "Magore's Daemonic Axe", 1, 3, 3, 3, -1, 1},
                m_goreaxe{Weapon::Type::Melee, "Goreaxe", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Redhand                          Yes
// Gorefists                        Yes
// No Respite                       Yes
// Blood Scent                      Yes
//

} // namespace Khorne
