/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosLordOnManticore : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Blade_And_Lance,
            Flail_And_Lance,
            Blade_And_Shield,
            Flail_And_Shield,
            Blade_And_Daggerfist,
            Flail_And_Daggerfist
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLordOnManticore();

        ~ChaosLordOnManticore() override = default;

        bool configure(WeaponOption weapon);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOption m_weapon = WeaponOption::Blade_And_Shield;

        Weapon m_blade,
                m_lance,
                m_flail,
                m_fangsAndClaws,
                m_tail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshield                 Yes
// Daemonbound                      Yes
// Chaos Lance                      Yes
// Daggerfist                       Yes
// Territorial Predator             Yes
// Iron-willed Overlord             TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
