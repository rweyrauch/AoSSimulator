/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SOULMASON_H
#define SOULMASON_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class MortisanSoulmason : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    MortisanSoulmason();
    ~MortisanSoulmason() override = default;

    bool configure();

protected:

private:


    Weapon m_staff,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Mortek Throne                    No
// Soul-guide                       No
//

} // namespace OssiarchBonereapers

#endif //SOULMASON_H
