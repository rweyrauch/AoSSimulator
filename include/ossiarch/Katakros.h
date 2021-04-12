/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class Katakros : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Katakros(Legion legion, bool isGeneral);

        Katakros() = delete;

        ~Katakros() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int woundsTaken() const {
            return wounds() - remainingWounds();
        }

        Weapon  m_indaKhaat{Weapon::Type::Melee, "Inda-Khaat", 1, 1, 3, 3, -3, 3},
                m_shieldImmortis{Weapon::Type::Melee, "The Shield Immortis", 1, 4, 3, 3, -2, 2},
                m_nadiriteDagger{Weapon::Type::Melee, "Nadirite Dagger", 1, 1, 3, 3, -1, 1},
                m_blades{Weapon::Type::Melee, "Nadirite Duelling Blades", 1, 6, 3, 3, -1, 1},
                m_greatblade{Weapon::Type::Melee, "Soulreaver Greatblade", 1, 3, 3, 3, -1, 1},
                m_spiritDagger{Weapon::Type::Melee, "Spirit Dagger", 1, 3, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Combination               Yes
// Mortarch of the Necropolis       TODO
// Nadirite Weapons                 Yes
// Endless Duty                     TODO
// Supreme Lord of the ...          TODO
// Aviarch Spymaster                TODO
// Gnosis Scrollbearer              TODO
// Prime Necrophoros                TODO
//

} // namespace OssiarchBonereapers

