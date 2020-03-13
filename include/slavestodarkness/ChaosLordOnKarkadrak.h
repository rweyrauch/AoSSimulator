/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDONDRAK_H
#define LORDONDRAK_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosLordOnKarkadrak : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 250;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ChaosLordOnKarkadrak();
    ~ChaosLordOnKarkadrak() override = default;

    bool configure();

protected:

    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_battleAxe,
        m_blade,
        m_hornsAndClaws,
        m_tail;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Brutish Rampage                  Yes
// Daemonbound                      Yes
// Fuelled by Carnage               TODO
// Rune-etched Plating              Yes
// The Knights of Chaos             TODO
//

} // SlavesToDarkness

#endif //LORDONDRAK_H