/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSWARRIORS_H
#define CHAOSWARRIORS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosWarriors : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    enum WeaponOption
    {
        HandWeaponAndShield = 0,
        HalberdAndShield,
        GreatBlade,
        PairedHandWeapons
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosWarriors();
    ~ChaosWarriors() override = default;

    bool configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    void onRestore() override;

protected:

    bool m_standardBearer = false;
    bool m_hornblower = false;
    bool m_pairedWeapons = false;
    bool m_hasShields = false;

private:

    Weapon m_handWeapons,
        m_halberd,
        m_greatBlade,
        m_handWeaponsChampion,
        m_halberdChampion,
        m_greatBladeChampion;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Chaos Runeshields                Yes
// Mark of Chaos                    No
// Berserk Fury                     Yes
// Legions of Chaos                 Yes
//

} // SlavesToDarkness

#endif //CHAOSWARRIORS_H