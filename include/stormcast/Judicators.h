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
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    enum WeaponOption
    {
        SkyboltBow,
        BoltstormCrossbow,
    };

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Judicators();

    ~Judicators() override = default;

    bool configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int extraAttacks(const Weapon *weapon) const override;

private:

    WeaponOption m_weaponOption = SkyboltBow;

    Weapon m_skyboltBow,
        m_skyboltPrime,
        m_boltstormCrossbow,
        m_shockboltBow,
        m_shockboltPrime,
        m_thunderboldCrossbow,
        m_stormGladius;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Chained Lightning                Yes
// Eternal Judgement                Yes
// Rapid Fire                       Yes
// Thunderbolt Crossbow             No
//

} // namespace StormcastEternals

#endif //JUDICATORS_H