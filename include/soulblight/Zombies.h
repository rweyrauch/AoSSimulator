/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {
    class Zombies : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Zombies(CursedBloodline bloodline, int numModels, int points);

        ~Zombies() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_zombieBite;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dragged Down and Torn Apart      Yes
// The Newly Dead                   Yes
//

} //namespace Soulblight
