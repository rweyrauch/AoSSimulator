/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_JUDICATORS_H
#define WARHAMMERSIM_JUDICATORS_H

#include <Unit.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Judicators : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;

    enum WeaponOption
    {
        SkyboltBow,
        BoltstormCrossbow,
    };

    Judicators();
    ~Judicators() override = default;

    bool configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows);

    bool hasKeyword(const std::string& word) const override;

protected:

    Rerolls toHitRerollsMissile(const Unit& unit) const override;
    int extraAttacksMissile() const override;

private:

    WeaponOption m_weaponOption = SkyboltBow;

    static Weapon s_skyboltBow,
                  s_boltstormCrossbow,
                  s_shockboltBow,
                  s_thunderboldCrossbow,
                  s_stormGladius;

    static std::vector<std::string> s_keywords;
};

} // namespace StormcastEternals

#endif //WARHAMMERSIM_JUDICATORS_H