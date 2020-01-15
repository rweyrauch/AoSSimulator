/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKWARBOSS_H
#define ORRUKWARBOSS_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class OrrukWarboss : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    enum WeaponOption
    {
        BossChoppaAndShield,
        PairedBossChoppas,
        MassiveChoppa,
        GreatWaaaghBanner
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    OrrukWarboss();
    ~OrrukWarboss() override = default;

    bool configure(WeaponOption weapon, bool warboar);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    WeaponOption m_weapon = BossChoppaAndShield;

    Weapon m_bossChoppa,
        m_massiveChoppa,
        m_greatWaaaghBanner,
        m_boarTusks;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Choppa Boss                      Yes
// Great Waaagh! Banner             No
// Boss Shield                      Yes
// Waaagh!                          No
//

} // namespace Greenskinz

#endif // ORRUKWARBOSS_H