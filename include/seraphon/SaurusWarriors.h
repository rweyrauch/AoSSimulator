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

    class SaurusWarriors : public SeraphonBase {
    public:

        enum WeaponOption {
            Celestite_Club,
            Celestite_Spear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusWarriors(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption weapons, bool iconBearer, bool wardrum);

        SaurusWarriors() = delete;

        ~SaurusWarriors() override;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls chargeRerolls() const override;

        int stardrakeIcon(const Unit *target);

    private:

        WeaponOption m_weaponOption = Celestite_Club;

        Weapon  m_celestiteClub{Weapon::Type::Melee, "Celestite Club", 1, 1, 4, 3, -1, 1},
                m_celestiteClubAlpha{Weapon::Type::Melee, "Celestite Club", 1, 2, 4, 3, -1, 1},
                m_celestiteSpear{Weapon::Type::Melee, "Celestite Spear", 2, 1, 4, 3, 0, 1},
                m_celestiteSpearAlpha{Weapon::Type::Melee, "Celestite Spear", 2, 2, 4, 3, 0, 1},
                m_jaws{Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   Yes
// Wardrum                          Yes
// Ordered Cohort                   Yes
//

} // namespace Seraphon
