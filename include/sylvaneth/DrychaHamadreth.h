/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRYCHAHAMADRETH_H
#define DRYCHAHAMADRETH_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class DrychaHamadreth : public SylvanethBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DrychaHamadreth();
    ~DrychaHamadreth() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onBeginRound(int battleRound) override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    bool m_enraged = false;

    Weapon m_colonyOfFlitterfuries,
        m_swarmOfSquirmlings,
        m_slashingTalons,
        m_thornedSlendervines;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deadly Infestation               Yes
// Mercurial Aspect                 Yes
// Song of Spite                    No
// Primal Terror                    No
//

} // namespace Sylvaneth

#endif //DRYCHAHAMADRETH_H