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

        VampireLord(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~VampireLord() override = default;

    protected:

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_blades;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       Yes
// Crimson Feast                    Yes
//

} // namespace Soulblight
