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

    class Kroxigor : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Kroxigor(WayOfTheSeraphon way, Constellation constellation, int numModels, int numMoonhammers, int points);

        Kroxigor() = delete;

        ~Kroxigor() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_maul{Weapon::Type::Melee, "Drakebite Maul", 2, 4, 4, 3, -1, 2},
                m_hammer{Weapon::Type::Melee, "Moon Hammer", 2, 0, 4, 3, -1, 2},
                m_jaws{Weapon::Type::Melee, "Vice-like Jaws", 1, 1, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Battle Synergy                   Yes
// Sweeping Blows                   Yes
// Jaws like a Steel Trap           Yes
//

} // namespace Seraphon
