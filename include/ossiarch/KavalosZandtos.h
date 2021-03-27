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

    class ArchKavalosZandtos : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ArchKavalosZandtos();

        ~ArchKavalosZandtos() override = default;

    protected:

        void configure();

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_lance,
                m_shield,
                m_hoovesAndTeeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Dark Lance                   Yes
// Hatred of the Living             Yes
// Unstoppable Charge               TODO
// Endless Duty                     TODO
// Still Their Breath               TODO
//

} // namespace OssiarchBonereapers

