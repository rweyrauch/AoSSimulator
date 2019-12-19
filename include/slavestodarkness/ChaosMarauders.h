/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSMARAUDERS_H
#define CHAOSMARAUDERS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosMarauders : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    enum WeaponOption
    {
        Flail = 0,
        AxeAndShield,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosMarauders();
    ~ChaosMarauders() override = default;

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
    bool m_drummer = false;
    bool m_weapons = WeaponOption::AxeAndShield;

private:

    Weapon m_axe,
        m_flail,
        m_axeChieftain,
        m_flailChieftain;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      No
// Drummer                          Yes
// Darkwood Shields                 Yes
// Mark of Chaos                    No
// Barbarian Hordes                 Yes
// Boundless Ferocity               No
//

} // SlavesToDarkness

#endif //CHAOSMARAUDERS_H