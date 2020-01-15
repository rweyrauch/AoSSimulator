/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WIGHTKINGBLADE_H
#define WIGHTKINGBLADE_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class WightKingWithBalefulTombBlade : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int BASESIZE_MOUNT = 60; // x35 oval
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    WightKingWithBalefulTombBlade();
    ~WightKingWithBalefulTombBlade() override = default;

    bool configure(bool hasSteed);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_hasSteed = false;

    Weapon m_balefulTombBlade,
        m_steedsHoovesAndTeeth;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Beheading Strike                 Yes
// Deathly Invocation               No
// Lord of Bones                    No
//

} // namespace Death

#endif // WIGHTKINGBLADE_H