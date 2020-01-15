/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GORDRAKK_H
#define GORDRAKK_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class GordrakkTheFistOfGork : public Ironjawz
{
public:
    static const int BASESIZE = 160;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 540;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    GordrakkTheFistOfGork();

    ~GordrakkTheFistOfGork() override = default;

    int move() const override;

    bool configure();

protected:

    void onWounded() override;
    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;

private:

    int getDamageTableIndex() const;

    int m_modelsSlainAtStartOfCombat = 0;

    Weapon m_bellow,
        m_smasha,
        m_kunnin,
        m_fistsAndTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Massively Destructive Bulk       Yes
// Kunnin                           Yes
// Smasha                           Yes
// Strength from Victory            Yes
// Voice of Gork                    No
//

} // namespace Ironjawz

#endif //GORDRAKK_H