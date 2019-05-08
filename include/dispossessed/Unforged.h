/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNFORGED_H
#define UNFORGED_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Unforged : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Unforged();
    ~Unforged() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_runicAxes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Runic Axes                       Yes
// Epic Deathblow                   No
// Nemesis                          Yes
// The Bigger They Are...           Yes
//

} // namespace Dispossessed

#endif //UNFORGED_H