/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAINRASPHORDE_H
#define CHAINRASPHORDE_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt
{

class ChainraspHorde : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ChainraspHorde();
    ~ChainraspHorde() override = default;

    bool configure(int numModels);

protected:

    Rerolls toWoundRerolls(const Weapon* weapon, const Unit* unit) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    static Weapon s_malignantWeapon,
        s_malignantWeaponWarden;

    static bool s_registered;
};

} // namespace Nighthaunt

#endif // CHAINRASPHORDE_H
