/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKHANTHEBLACK_H
#define ARKHANTHEBLACK_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class ArkhanTheBlack : public LegionOfNagashBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ArkhanTheBlack();
    ~ArkhanTheBlack() override = default;

    bool configure();

    int castingModifier() const override;

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int unbindingModifier() const override;

private:

    Weapon m_zefetKar,
        m_khenashAn,
        m_ebonClaws,
        m_clawsAndDaggers;

    static bool s_registered;

};

//
// Abilities                    Implemented
// -------------------------------------------
// Feaster of Souls                 TODO
// Frightful Touch                  Yes
// Staff of Spirits                 TODO
// Mortarch of Sacrament            TODO
// Curse of Years                   TODO
// First of the Mortarchs           TODO
//

} // namespace Death

#endif // ARKHANTHEBLACK_H