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

        SoulGrinder(DamnedLegion legion, MarkOfChaos mark, WeaponOption option);

        SoulGrinder() = delete;

        ~SoulGrinder() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon  m_cannon{Weapon::Type::Missile, "Harvester Cannon", 16, 6, 4, 3, -1, 1},
                m_phlegm{Weapon::Type::Missile, "Phlegm Bombardment", 20, 1, 4, 3, -2, 3},
                m_legs{Weapon::Type::Melee, "Piston-driven Legs", 1, 6, 4, 3, -1, 1},
                m_claw{Weapon::Type::Melee, "Hellforged Claw", 2, 1, 4, 3, -2, RAND_D6},
                m_blade{Weapon::Type::Melee, "Warpmetal Blade", 2, 2, 4, 3, -2, 3},
                m_talon{Weapon::Type::Melee, "Daemonbone Talon", 2, 4, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hellforged Claw                  Yes
// Implacable Advance               Yes
//

} // SlavesToDarkness

