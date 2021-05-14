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

    class CorpseCartWithUnholyLodestone : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit CorpseCartWithUnholyLodestone(Legion legion);

        ~CorpseCartWithUnholyLodestone() override;

    protected:

        int unholyLodestoneCastingMod(const Unit *caster);

    private:

        Weapon m_goad,
                m_lash,
                m_blades;

        lsignal::slot m_lodestoneSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unholy Lodestone                 Yes
// Locus of Undeath                 TODO
// Cryptsword                       TODO
//

} // namespace Soulblight
