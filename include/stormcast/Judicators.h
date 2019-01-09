/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef JUDICATORS_H
#define JUDICATORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Judicators : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    enum WeaponOption
    {
        SkyboltBow,
        BoltstormCrossbow,
    };

    Judicators();
    ~Judicators() override = default;

    bool configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows);

protected:

    Rerolls toHitRerollsMissile(const Unit* unit) const override;
    int extraAttacksMissile() const override;

private:

    WeaponOption m_weaponOption = SkyboltBow;

    static Weapon s_skyboltBow,
                  s_boltstormCrossbow,
                  s_shockboltBow,
                  s_thunderboldCrossbow,
                  s_stormGladius;
};

} // namespace StormcastEternals

#endif //JUDICATORS_H