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

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Katakros();

        ~Katakros() override = default;

    protected:

        void configure();

        void onWounded() override;

        void onRestore() override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int woundsTaken() const {
            return wounds() - remainingWounds();
        }

        Weapon m_indaKhaat,
                m_shieldImmortis,
                m_nadiriteDagger,
                m_blades,
                m_greatblade,
                m_spiritDagger;

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

