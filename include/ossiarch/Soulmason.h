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

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Mortek Throne                    TODO
// Soul-guide                       TODO
//

} // namespace OssiarchBonereapers

#endif //SOULMASON_H
