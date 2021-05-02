/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Stormfiends : public Skaventide {
    public:

        enum WeaponOption1 {
            Warpfire_Projectors,
            Windlaunchers
        };
        enum WeaponOption2 {
            Grinderfists,
            Ratling_Cannons
        };
        enum WeaponOption3 {
            Doomflayer_Gauntlets,
            Shock_Gauntlets
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Stormfiends(int points);

        ~Stormfiends() override = default;

        bool configure(int numModels, WeaponOption1 weapon1, WeaponOption2 weapon2, WeaponOption3 weapon3);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        WeaponOption1 m_weapons1 = Warpfire_Projectors;
        WeaponOption2 m_weapons2 = Grinderfists;
        WeaponOption3 m_weapons3 = Doomflayer_Gauntlets;

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
