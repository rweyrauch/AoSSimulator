/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSMARAUDERHORSEMEN_H
#define CHAOSMARAUDERHORSEMEN_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosMarauderHorsemen : public Unit
{
public:

    static const int BASESIZE = 65;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    enum WeaponOption
    {
        AxeAndShield = 0,
        JavelinAndShield,
        Flail,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosMarauderHorsemen();
    ~ChaosMarauderHorsemen() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool drummer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    int runModifier() const override;
    int chargeModifier() const override;
    void onRestore() override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

protected:

    bool m_iconBearer = false;
    bool m_hornblower = false;
    bool m_weapons = WeaponOption::AxeAndShield;

private:

    Weapon m_javelinMissile,
        m_axe,
        m_flail,
        m_javelin,
        m_axeMaster,
        m_flailMaster,
        m_javelinMaster,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      No
// Hornblower                       Yes
// Darkwood Shields                 Yes
// Mark of Chaos                    No
// Barbarian Hordes                 Yes
// Feigned Flight                   Yes
//

} // SlavesToDarkness

#endif //CHAOSMARAUDERHORSEMEN_H