/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMFIENDS_H
#define STORMFIENDS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Stormfiends : public Skaventide
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 260;
    static const int POINTS_MAX_UNIT_SIZE = 260*3;

    enum WeaponOption_1
    {
        WarpfireProjectors,
        Windlaunchers
    };
    enum WeaponOption_2
    {
        Grinderfists,
        RatlingCannons
    };
    enum WeaponOption_3
    {
        DoomflayerGauntlets,
        ShockGauntlets
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Stormfiends();
    ~Stormfiends() override = default;

    bool configure(int numModels, WeaponOption_1 weapon1, WeaponOption_2 weapon2, WeaponOption_3 weapon3);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    WeaponOption_1 m_weapons1 = WarpfireProjectors;
    WeaponOption_2 m_weapons2 = Grinderfists;
    WeaponOption_3 m_weapons3 = DoomflayerGauntlets;

    Weapon m_ratlingCannons,
        m_windlaunchers,
        m_warpfireProjectors,
        m_doomfireGauntlets,
        m_grinderfists,
        m_shockGauntlets,
        m_clubbingBlows;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Doomflayer Gauntlets             Yes
// Grinderfist Tunnellers           TODO
// Shock Gauntlets                  Yes
// Warpfire Projectors              TODO
// Warpstone-laced Armour           Yes
// Windlaunchers                    Yes
//

} // namespace Skaven

#endif //STORMFIENDS_H