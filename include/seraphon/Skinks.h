/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Skinks : public SeraphonBase {
    public:

        enum WeaponOption {
            Javelins_Daggers_And_Bucklers,
            Boltspitters_And_Clubs,
            Boltspitters_Daggers_And_Bucklers,
            Clubs_And_Bucklers
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Skinks(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption weapons);

        Skinks() = delete;

        ~Skinks() override = default;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOption m_weaponOption = Boltspitters_Daggers_And_Bucklers;

        Weapon  m_javelin{Weapon::Type::Missile, "Meteoric Javelin", 8, 1, 5, 4, 0, 1},
                m_boltspitter{Weapon::Type::Missile, "Boltspitter", 16, 1, 5, 5, 0, 1},
                m_dagger{Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1},
                m_daggerAlpha{Weapon::Type::Melee, "Celestite Dagger", 1, 2, 5, 5, 0, 1},
                m_club{Weapon::Type::Melee, "Moonstone Club", 1, 1, 4, 4, 0, 1},
                m_clubAlpha{Weapon::Type::Melee, "Moonstone Club", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Star-buckler                     Yes
// Swarming Cohort                  Yes
//

} // namespace Seraphon
