/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSKNIGHTS_H
#define CHAOSKNIGHTS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosKnights : public Unit
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    enum WeaponOption
    {
        EnsorcelledWeapon = 0,
        ChaosGlaive,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosKnights();
    ~ChaosKnights() override = default;

    bool configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

protected:

    bool m_standardBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_ensorcelledWeapon,
        m_glaive,
        m_ensorcelledWeaponLeader,
        m_glaiveLeader,
        m_hooves;

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
// Impaling Charge                  Yes
// Terrifying Champions             No
//

} // SlavesToDarkness

#endif //CHAOSKNIGHTS_H