/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSWARRIORS_H
#define SAURUSWARRIORS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusWarriors : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    enum WeaponOption
    {
        CelestiteClub,
        CelestiteSpear
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    SaurusWarriors();
    ~SaurusWarriors() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool wardrum);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toSaveModifier(const Weapon *weapon) const override;
    int runModifier() const override { if (m_wardrum) return 5; return 0; }

private:

    WeaponOption m_weaponOption = CelestiteClub;
    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestiteClub,
        m_celestiteClubAlpha,
        m_celestiteSpear,
        m_celestiteSpearAlpha,
        m_jawsAndShield;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   No
// Wardrum                          Yes
// Stardrake Shields                Yes
// Ordered Cohort                   Yes
//

} // namespace Seraphon

#endif //SAURUSWARRIORS_H