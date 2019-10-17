/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NECROMANCER_H
#define NECROMANCER_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class Necromancer : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Necromancer();
    ~Necromancer() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_staff;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Undead Minions                   No
// Deathly Invocation               No
// Vanhel's Danse Macabre           No
//

} // namespace Death

#endif // NECROMANCER_H