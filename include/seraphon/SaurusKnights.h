/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSKNIGHTS_H
#define SAURUSKNIGHTS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusKnights : public SeraphonBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    enum WeaponOption
    {
        CelestiteBlade,
        CelestiteWarspear
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    SaurusKnights();
    ~SaurusKnights() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool wardrum);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls chargeRerolls() const override;

private:

    WeaponOption m_weaponOption = CelestiteBlade;
    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestiteBlade,
        m_celestiteBladeAlpha,
        m_celestiteSpear,
        m_celestiteSpearAlpha,
        m_jaws,
        m_coldOneJaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   No
// Wardrum                          Yes
// Celestite Warspear               Yes
//

} // namespace Seraphon

#endif //SAURUSKNIGHTS_H