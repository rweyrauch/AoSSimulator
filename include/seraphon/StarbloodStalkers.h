/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class StarbloodStalkers : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        StarbloodStalkers();

        ~StarbloodStalkers() override = default;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_javelin{Weapon::Type::Missile, "Meteoric Javelin", 8, 1, 5, 4, 0, 1},
                m_dartpipe{Weapon::Type::Missile, "Dartpipe", 16, 2, 3, 4, 0, 1},
                m_boltspitter{Weapon::Type::Missile, "Boltspitter", 16, 1, 5, 5, 0, 1},
                m_dagger{Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1},
                m_club{Weapon::Type::Melee, "Moonstone Club", 1, 1, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chameleon Ambush                 TODO
// Star-buckler                     Yes
// Star-venom                       Yes
//

} // namespace Seraphon
