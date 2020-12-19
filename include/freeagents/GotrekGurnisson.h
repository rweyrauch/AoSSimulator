/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOTREKGURNISSON_H
#define GOTREKGURNISSON_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace FreeAgent {

    class GotrekGurnisson : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GotrekGurnisson();

        ~GotrekGurnisson() override = default;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds onEndCombat(PlayerId player) override;

    private:

        Weapon m_zangromThaz;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Avatar of Grimnir                Yes
// Krag Blackhammer's Master Rune   Yes
// Unstoppable Battle Fury          Yes
// Shoulder Plate of Edassa         Yes
//

} // namespace FreeAgent

#endif //GOTREKGURNISSON_H