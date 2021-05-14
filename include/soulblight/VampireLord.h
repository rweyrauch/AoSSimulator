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

    class VampireLord : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        VampireLord(Legion legion, bool withSteed, bool withWings, bool chalice, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~VampireLord() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_blades,
                m_hoovesAndTeeth;

        bool m_haveChaliceOfBlood = false;
        bool m_usedChaliceOfBlood = false;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       Yes
// Chalice of Blood                 Yes
// Deathly Invocation               Yes
// Blood Feast                      TODO
//

} // namespace Soulblight
