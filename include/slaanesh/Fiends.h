/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FIENDS_H
#define FIENDS_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class Fiends : public Unit
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 210;
    static const int POINTS_MAX_UNIT_SIZE = 630;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Fiends();
    ~Fiends() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_deadlyPincers,
        m_deadlyPincersBlissbringer,
        m_barbedStinger;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crushing Grip                    Yes
// Deadly Venom                     Yes
// Disruptive Song                  No
// Soporific Musk                   Yes

} // Slannesh

#endif //FIENDS_H