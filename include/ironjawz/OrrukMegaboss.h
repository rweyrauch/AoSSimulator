/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKMEGABOSS_H
#define ORRUKMEGABOSS_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class OrrukMegaboss : public Ironjawz
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 150;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    OrrukMegaboss();

    ~OrrukMegaboss() override = default;

    bool configure();

protected:

    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;
    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;

private:

    Weapon m_bossChoppaAndFist;
    int m_modelsSlainAtStartOfCombat = 0;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Go on Ladz, Get Stuck In!        No
// Strength from Victory            Yes
// Rip-toof Fist                    Yes
//

} // namespace Ironjawz

#endif //ORRUKMEGABOSS_H