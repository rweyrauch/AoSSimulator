/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSKNIGHTS_H
#define SAURUSKNIGHTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusKnights : public Unit
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOption
    {
        CelestiteBlade,
        CelestiteLance
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    SaurusKnights();
    ~SaurusKnights() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool wardrum);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon *weapon) const override;

private:

    WeaponOption m_weaponOption = CelestiteBlade;
    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestiteBlade,
        m_celestiteBladeAlpha,
        m_celestiteLance,
        m_celestiteLanceAlpha,
        m_jawsAndShield,
        m_coldOneBite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   No
// Wardrum                          No
// Stardrake Shields                Yes
// Blazing Lances                   Yes
//

} // namespace Seraphon

#endif //SAURUSKNIGHTS_H