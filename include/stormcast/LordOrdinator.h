/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_LORDORDINATOR_H
#define WARHAMMERSIM_LORDORDINATOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordOrdinator : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;

    enum WeaponOption
    {
        AstralHammers,
        AstralGrandhammer
    };

    LordOrdinator();
    ~LordOrdinator() override = default;

    bool configure(WeaponOption weaponOption);

protected:

private:

    WeaponOption m_weaponOption = AstralHammers;

    static Weapon s_astralHammers,
        s_astralGrandhammer;
};

} // namespace StormcastEternals

#endif //WARHAMMERSIM_LORDORDINATOR_H