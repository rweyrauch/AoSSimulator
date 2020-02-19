/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRETYRANTS_H
#define SPIRETYRANTS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class SpireTyrants : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 9;
    static const int MAX_UNIT_SIZE = 36;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SpireTyrants();
    ~SpireTyrants() override = default;

    bool configure(int numModels);


protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_gladiatorialWeapons,
        m_gladiatorialWeaponsChampion,
        m_gladiatorialWeaponsHeadclaimer,
        m_gladiatorialWeaponsDestroyer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Pit Fighters                     Yes
//

} // SlavesToDarkness


#endif //SPIRETYRANTS_H