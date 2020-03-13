/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSKNIGHTS_H
#define CHAOSKNIGHTS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosKnights : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    enum WeaponOption
    {
        EnsorcelledWeapon = 0,
        CursedLance,
        CursedFlail,    // Doom Knight only
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    ChaosKnights();
    ~ChaosKnights() override;

    bool configure(int numModels, WeaponOption weapons, WeaponOption doomKnightWeapon, bool standardBearer, bool hornblower);

protected:

    void onWounded() override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

    int terrifyingChampions(const Unit* unit);

protected:

    bool m_standardBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_ensorcelledWeapon,
        m_lance,
        m_ensorcelledWeaponLeader,
        m_lanceLeader,
        m_flailLeader,
        m_hooves;

    lsignal::slot m_terrifyingSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Chaos Runeshields                Yes
// Mark of Chaos                    Yes
// Impaling Charge                  Yes
// Terrifying Champions             Yes
//

} // SlavesToDarkness

#endif //CHAOSKNIGHTS_H