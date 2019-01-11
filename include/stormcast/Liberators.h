/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LIBERATORS_H
#define LIBERATORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Liberators : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;

    enum WeaponOption
    {
        Warhammer,
        Warblade,
    };

    Liberators();
    ~Liberators() override = default;

    bool configure(int numModels, WeaponOption weapons, bool pairedWeapons, int numGrandhammers, int numGrandblades);

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Hits applyHitModifiers(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    WeaponOption m_weaponOption = Warhammer;
    bool m_pairedWeapons = false;

    static Weapon s_warhammer,
                  s_warhammerPrime,
                  s_warblade,
                  s_warbladePrime,
                  s_grandhammer,
                  s_grandblade;

};

} // namespace StormcastEternals

#endif //LIBERATORS_H