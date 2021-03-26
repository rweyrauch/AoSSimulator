/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class SoulGrinder : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Warpmetal_Blade,
            Daemonbone_Talon
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SoulGrinder();

        ~SoulGrinder() override = default;

        bool configure(WeaponOption option);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_cannon,
                m_phlegm,
                m_legs,
                m_claw,
                m_blade,
                m_talon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hellforged Claw                  Yes
// Implacable Advance               Yes
//

} // SlavesToDarkness

