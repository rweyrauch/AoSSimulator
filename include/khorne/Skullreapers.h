/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLREAPERS_H
#define SKULLREAPERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Skullreapers : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Skullreapers();
    ~Skullreapers() override = default;

    bool configure(int numModels, bool iconBearer);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    bool battleshockRequired() const override { return false; }
    void onModelSlain() override;

private:

    bool m_iconBearer = false;

    Weapon m_blades,
        m_viciousMutation;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Trial of Skulls                  Yes
// Daemonforged Weapons             Yes
// Murderous to the Last            Yes
//

} // namespace Khorne

#endif //SKULLREAPERS_H