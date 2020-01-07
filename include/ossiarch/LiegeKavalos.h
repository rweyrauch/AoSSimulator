/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LIEGEKAVALOS_H
#define LIEGEKAVALOS_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class LiegeKavalos : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 80;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    LiegeKavalos();
    ~LiegeKavalos() override = default;

    bool configure();

protected:


private:

    Weapon m_blade,
        m_shield,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Charge               No
// Endless Duty                     No
//

} // namespace OssiarchBonereapers

#endif //LIEGEKAVALOS_H