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

    class Razordons : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Razordons(WayOfTheSeraphon way, Constellation constellation, int numModels, int points);

        Razordons() = delete;

        ~Razordons() override = default;

    protected:

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_spikes{Weapon::Type::Missile, "Volley of Spikes", 18, RAND_2D6, 3, 4, 0, 1},
                m_tail{Weapon::Type::Melee, "Spiked Tail", 1, 3, 3, 3, -2, 2},
                m_goad{Weapon::Type::Melee, "Celestite Goad", 1, 1, 4, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Piercing Barbs                   Yes
// Instinctive Defense              TODO
//

} // namespace Seraphon
