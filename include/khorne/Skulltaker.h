/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLTAKER_H
#define SKULLTAKER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Skulltaker : public KhorneBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Skulltaker();
    ~Skulltaker() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_slayerSword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Skulls                  Yes
// Decapitating Strike              Yes
// Heroes' Bane                     Yes
// Heads Must Roll                  TODO
//

} // namespace Khorne

#endif //SKULLTAKER_H