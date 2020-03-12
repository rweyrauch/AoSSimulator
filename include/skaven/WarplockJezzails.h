/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARPLOCKJEZZAILS_H
#define WARPLOCKJEZZAILS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarplockJezzails : public Skaventide
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    WarplockJezzails();
    ~WarplockJezzails() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    int toSaveModifier(const Weapon *weapon) const override;

private:

    Weapon m_jezzail,
        m_knives;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Warpstone Snipers                Yes
// Pavise                           Yes
//

} // namespace Skaven

#endif //WARPLOCKJEZZAILS_H