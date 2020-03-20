/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDEXECUTIONER_H
#define LORDEXECUTIONER_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class LordExecutioner : public Nighthaunt
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    LordExecutioner();
    ~LordExecutioner() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartCombat(PlayerId player) override;
    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_greataxe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Beheading Strike                 Yes
// Staring Death in the Face        Yes
// Disembodied Skulls               Yes
//


} // namespace Nighthaunt

#endif // LORDEXECUTIONER_H
