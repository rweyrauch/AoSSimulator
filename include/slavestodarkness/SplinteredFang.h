/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPLINTEREDFANG_H
#define SPLINTEREDFANG_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class SplinteredFang : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    SplinteredFang();
    ~SplinteredFang() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onWounded() override;

protected:

    bool m_hasSerpentCaller = true;

private:

    Weapon m_poisonedWeapons,
        m_poisonedWeaponsLeader;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Snake Charmer                    No
// One Cut, One Kill                Yes
//

} // SlavesToDarkness

#endif //SPLINTEREDFANG_H