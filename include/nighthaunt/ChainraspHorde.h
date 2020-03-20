/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAINRASPHORDE_H
#define CHAINRASPHORDE_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class ChainraspHorde : public Nighthaunt
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ChainraspHorde();
    ~ChainraspHorde() override = default;

    bool configure(int numModels);

protected:

    Rerolls toWoundRerolls(const Weapon* weapon, const Unit* unit) const override;

private:

    Weapon m_malignantWeapon,
        m_malignantWeaponWarden;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Chilling Horde                   Yes

} // namespace Nighthaunt

#endif // CHAINRASPHORDE_H
