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

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Judicators();
    ~Judicators() override = default;

    bool configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int extraAttacks(const Weapon* weapon) const override;

private:

    WeaponOption m_weaponOption = SkyboltBow;

    static Weapon s_skyboltBow,
                  s_skyboltPrime,
                  s_boltstormCrossbow,
                  s_shockboltBow,
                  s_shockboltPrime,
                  s_thunderboldCrossbow,
                  s_stormGladius;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //JUDICATORS_H