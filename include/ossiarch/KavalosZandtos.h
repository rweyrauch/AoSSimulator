/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KAVALOSZANDTOS_H
#define KAVALOSZANDTOS_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class ArchKavalosZandtos : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 80;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    ArchKavalosZandtos();
    ~ArchKavalosZandtos() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_lance,
        m_shield,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Dark Lance                   Yes
// Hatred of the Living             Yes
// Unstoppable Charge               No
// Endless Duty                     No
// Still Their Breath               No
//

} // namespace OssiarchBonereapers

#endif //KAVALOSZANDTOS_H
