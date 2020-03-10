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
    static const int BASESIZE = 100;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Feaster of Souls                 No
// Frightful Touch                  Yes
// Staff of Spirits                 No
// Mortarch of Sacrament            No
// Curse of Years                   No
// First of the Mortarchs           No
//

} // namespace Death

#endif // ARKHANTHEBLACK_H