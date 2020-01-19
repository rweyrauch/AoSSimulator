/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GAUNTSUMMONER_H
#define GAUNTSUMMONER_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class GauntSummonerOfTzeentch : public TzeentchBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    GauntSummonerOfTzeentch();
    ~GauntSummonerOfTzeentch() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_staff,
        m_blade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Warptongue Blade                 Yes
// Book of Profane Secrets          No
// Infernal Flames                  No
//

} // namespace Tzeentch

#endif //GAUNTSUMMONER_H
