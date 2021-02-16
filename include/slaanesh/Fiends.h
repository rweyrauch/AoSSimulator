/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class Fiends : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Fiends();

        ~Fiends() override;

        bool configure(int numModels);

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int disruptiveSong(const Unit *caster);

    private:

        Weapon m_deadlyPincers,
                m_deadlyPincersBlissbringer,
                m_barbedStinger;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crushing Grip                    Yes
// Deadly Venom                     Yes
// Disruptive Song                  Yes
// Soporific Musk                   Yes

} // Slannesh
