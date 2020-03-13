/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLCANNONS_H
#define SKULLCANNONS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class SkullCannons : public KhorneBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * MAX_UNIT_SIZE;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SkullCannons();
    ~SkullCannons() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_burningSkulls,
        m_hellblades,
        m_gnashingMaw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Burning Skulls                   Yes
// Grind their Bones, Seize their Skulls    TODO
// Decapitating Blow                Yes
//

} // namespace Khorne

#endif //SKULLCANNONS_H