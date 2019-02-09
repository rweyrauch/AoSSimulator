/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDORDINATOR_H
#define LORDORDINATOR_H

#include <vector>
#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordOrdinator : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    enum WeaponOption
    {
        AstralHammers,
        AstralGrandhammer
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    LordOrdinator();
    ~LordOrdinator() override = default;

    bool configure(WeaponOption weaponOption);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartCombat(PlayerId player) override;
    void onEndCombat(PlayerId player) override;

private:

    WeaponOption m_weaponOption = AstralHammers;

    Weapon m_astralHammers,
        m_astralGrandhammer;

    mutable std::vector<const Unit*> m_meteoricSlam;  // Modified in weaponDamage method

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Engineer                  No
// Comet Strike                     Yes
// Meteoric Slam                    Yes
// Solemn Duty                      No
//

} // namespace StormcastEternals

#endif //LORDORDINATOR_H