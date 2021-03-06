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

    class PlagueMonks : public Skaventide {
    public:

        enum WeaponOptions {
            Paired_Foetid_Blades,
            Foetid_Blade_And_Woe_Stave
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        PlagueMonks();

        ~PlagueMonks() override = default;

        bool configure(int numModels, WeaponOptions weapons, int contagionBanners, int iconsOfPestilence, int doomGongs,
                       int baleChimes);

    protected:

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        WeaponOptions m_weaponOption = Paired_Foetid_Blades;
        int m_numContagionBanners = 0;
        int m_numIconsOfPestilence = 0;
        int m_numDoomGongs = 0;
        int m_numBaleChimes = 0;

        Weapon m_foetidBlade,
                m_woeStave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contagion Banner                 TODO
// Icon of Pestilence               Yes
// Doom Gong                        Yes
// Bale-chime                       Yes
// Pair of Foetid Blades            Yes
// Frenzied Assault                 Yes
// Book of Woes                     Yes

} // namespace Skaven
