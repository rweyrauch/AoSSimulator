/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAGASH_H
#define NAGASH_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class Nagash : public LegionOfNagashBase
{
public:
    static const int BASESIZE = 100;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 880;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Nagash();
    ~Nagash() override = default;

    bool configure();

    int castingModifier() const override;

protected:

    int getDamageTableIndex() const;

    int unbindingModifier() const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_gaze,
        m_alakanash,
        m_zefetNebtar,
        m_clawsAndDaggers;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Alakanash, the Staff of Power    No
// Invocation of Nagash             No
// Frightful Touch                  Yes
// Morikhane                        No
// The Nine Books of Nagash         Yes
// Hand of Dust                     No
// Soul Stealer                     No
// Supreme Lord of the Undead       No
//

} // namespace Death

#endif // NAGASH_H