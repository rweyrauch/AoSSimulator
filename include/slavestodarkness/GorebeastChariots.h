/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOREBEASTCHARIOTS_H
#define GOREBEASTCHARIOTS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class GorebeastChariots : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    enum WeaponOption
    {
        GreatBladeAndWhip,
        WarFlailAndWhip
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    GorebeastChariots();
    ~GorebeastChariots() override = default;

    bool configure(int numModels, WeaponOption weapons);

protected:

    void onCharged() override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_greatBlade,
        m_flail,
        m_whip,
        m_greatBladeExalted,
        m_flailExalted,
        m_whipExalted,
        m_fists;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Crashing Charge                  Yes
// Explosive Brutality              Yes
//

} // SlavesToDarkness

#endif //GOREBEASTCHARIOTS_H