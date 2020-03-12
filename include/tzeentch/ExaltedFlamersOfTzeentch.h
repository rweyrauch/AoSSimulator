/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EXALTEDFLAMERSOFTZEENTCH_H
#define EXALTEDFLAMERSOFTZEENTCH_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class ExaltedFlamersOfTzeentch : public TzeentchBase
{
public:

    static const int BASESIZE = 75; // x42
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ExaltedFlamersOfTzeentch();
    ~ExaltedFlamersOfTzeentch() override = default;

    bool configure(int numModels);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

private:

    Weapon m_warpflame,
        m_flamingMaw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Capricious Warpflame             Yes
// Touched by Fire                  Yes
//

} // Tzeentch

#endif //EXALTEDFLAMERSOFTZEENTCH_H