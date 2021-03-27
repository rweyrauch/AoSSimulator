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

        void configure();

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_javelin,
                m_dartpipe,
                m_boltspitter,
                m_dagger,
                m_club;

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
