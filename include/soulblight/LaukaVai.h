/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class LaukaVai : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LaukaVai(Lore lore, bool isGeneral);

        ~LaukaVai() override = default;

    protected:

        void onEndCombat(PlayerId player) override;

        void onCharged() override;

    private:

        Weapon  m_rapier{Weapon::Type::Melee, "Askurga Rapier", 1, 4, 3, 3, -1, 2},
                m_talons{Weapon::Type::Melee, "Gore-drenched Talons", 3, 3, 4, 3, -1, RAND_D6},
                m_tail{Weapon::Type::Melee, "Impaling Tail", 1, RAND_D6, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Champion of the Avengorii        TODO
// Nightmare's Miasma               TODO
// Undeniable Impulse               TODO
// The Hunger                       Yes
// Death's Downpour                 Yes
// A Queen Amongst Monsters         TODO
//

} // namespace Soulblight
