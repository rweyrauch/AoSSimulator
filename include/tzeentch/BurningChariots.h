/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BURNINGCHARIOTS_H
#define BURNINGCHARIOTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class BurningChariotsOfTzeentch : public Unit
{
public:

    static const int BASESIZE = 120;  //x92
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BurningChariotsOfTzeentch();
    ~BurningChariotsOfTzeentch() override = default;

    bool configure(int numModels);

protected:

    Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_warpflame,
        m_bite,
        m_jabs,
        m_flamingMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Capricious Warpflame             Yes
// Touched by Fire                  Yes
// Wake of Fire                     No
// Sky-sharks                       Yes
//

} // Tzeentch

#endif //BURNINGCHARIOTS_H