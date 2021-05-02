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

    class SaurusKnights : public SeraphonBase {
    public:

        enum WeaponOption {
            Celestite_Blade,
            Celestite_Warspear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SaurusKnights(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption weapons, bool iconBearer, bool wardrum, int points);

        SaurusKnights() = delete;

        ~SaurusKnights() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls chargeRerolls() const override;

        int stardrakeIcon(const Unit *target);

    private:

        WeaponOption m_weaponOption = Celestite_Blade;

        Weapon  m_celestiteBlade{Weapon::Type::Melee, "Celestite Blade", 1, 2, 3, 3, 0, 1},
                m_celestiteBladeAlpha{Weapon::Type::Melee, "Celestite Blade", 1, 3, 3, 3, 0, 1},
                m_celestiteSpear{Weapon::Type::Melee, "Celestite Warspear", 1, 2, 4, 3, 0, 1},
                m_celestiteSpearAlpha{Weapon::Type::Melee, "Celestite Warspear", 1, 3, 4, 3, 0, 1},
                m_jaws{Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1},
                m_coldOneJaws{Weapon::Type::Melee, "Snapping Jaws", 1, 2, 3, 4, 0, 1};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   Yes
// Wardrum                          Yes
// Celestite Warspear               Yes
//

} // namespace Seraphon
