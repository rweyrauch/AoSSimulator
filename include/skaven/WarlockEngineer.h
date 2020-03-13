/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARLOCKENGINEER_H
#define WARLOCKENGINEER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarlockEngineer : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    WarlockEngineer();
    ~WarlockEngineer() override = default;

    bool configure();

protected:

    void onRestore() override;
    Wounds onEndCombat(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_pistol,
           m_blade;

    mutable bool m_moreMoreFailed = false;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// More-more Warp-energy!           Yes
// Warp Lightning                   Yes
//

} // namespace Skaven

#endif //WARLOCKENGINEER_H