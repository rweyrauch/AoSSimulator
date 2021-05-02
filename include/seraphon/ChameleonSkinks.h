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

    class ChameleonSkinks : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChameleonSkinks(WayOfTheSeraphon way, Constellation constellation, int numModels, int points);

        ChameleonSkinks() = delete;

        ~ChameleonSkinks() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_dartpipe{Weapon::Type::Missile, "Dartpipe", 16, 2, 3, 4, 0, 1},
                m_dagger{Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chameleon Ambush                 TODO
// Perfect Mimicry                  TODO
// Star-venom                       Yes
//

} // namespace Seraphon
