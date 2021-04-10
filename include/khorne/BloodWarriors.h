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

    class BloodWarriors : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodWarriors() = delete;

        ~BloodWarriors() override = default;

    protected:

        BloodWarriors(SlaughterHost host, int numModels, bool pairedGoreax, int numGoreglaives, bool iconBearer);

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int braveryModifier() const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        bool m_pairedGoreaxe = false;

        Weapon  m_goreaxe{Weapon::Type::Melee, "Goreaxe", 1, 2, 3, 4, 0, 1},
                m_goreaxeChampion{Weapon::Type::Melee, "Goreaxe", 1, 3, 3, 4, 0, 1},
                m_goreglaive{Weapon::Type::Melee, "Goreglaive", 1, 2, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// No Respite                       Yes
// Goreaxes                         Yes
// Gorefists                        Yes

} // namespace Khorne
