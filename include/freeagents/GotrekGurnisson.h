/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOTREK_H
#define GOTREK_H

#include <Unit.h>
#include <Weapon.h>

namespace FreeAgent
{

class GotrekGurnisson : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 520;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    GotrekGurnisson();
    ~GotrekGurnisson() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds onEndCombat(PlayerId player) override;

private:

    Weapon m_zangromThaz;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Avatar of Grimnir                Yes
// Krag Blackhammer's Master Rune   Yes
// Unstoppable Battle Fury          Yes
// Shoulder Plate of Edassa         Yes
//

} // namespace FreeAgent

#endif //GOTREK_H