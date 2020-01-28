/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSOLDBLOOD_H
#define SAURUSOLDBLOOD_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusOldblood : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    enum WeaponOption
    {
        SuntoothMaul,
        CelestiteWarblade,
        CelestiteWarspear,
        CelestiteGreatblade
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SaurusOldblood();
    ~SaurusOldblood() override = default;

    bool configure(WeaponOption option);

protected:

private:

    Weapon m_maul,
        m_warblade,
        m_warspear,
        m_greatblade,
        m_jawsAndShield;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Shield                 No
// Wrath of the Seraphon            No
// Paragon of Order                 No
//

} // namespace Seraphon

#endif //SAURUSOLDBLOOD_H