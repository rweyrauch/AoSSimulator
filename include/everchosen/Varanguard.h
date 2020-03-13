/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VARANGUARD_H
#define VARANGUARD_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class Varanguard : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 75; //x42 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 300;
    static const int POINTS_MAX_UNIT_SIZE = 300*4;

    enum WeaponOption
    {
        EnsorcelledWeapon,
        Fellspear,
        DaemonforgedBlade
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Varanguard();
    ~Varanguard() override = default;

    bool configure(int numModels, WeaponOption weapon);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_ensorcelledWeapon,
        m_fellspear,
        m_blade,
        m_fangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    TODO
// Daemonbound                      Yes
// Favoured of the Everchosen       TODO
// Impaling Charge                  Yes
// Relentless Killers               TODO
// Warpsteel Shields                TODO
//

} // namespace SlavesToDarkness

#endif //VARANGUARD_H