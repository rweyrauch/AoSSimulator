/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NEAVEBLACKTALON_H
#define NEAVEBLACKTALON_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class NeaveBlacktalon : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    NeaveBlacktalon();
    ~NeaveBlacktalon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_boltstormPistol,
        m_whirlwindAxes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lightning-fast Strikes           Yes
// Tireless Hunter                  Yes
// Nemesis                          Yes
// Windrider                        No
//

} // namespace StormcastEternals

#endif //NEAVEBLACKTALON_H