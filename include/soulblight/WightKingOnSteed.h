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

    class WightKingOnSteed : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WightKingOnSteed(CursedBloodline bloodline, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WightKingOnSteed() override = default;

    protected:

        void onCharged() override;

    private:

        Weapon  m_lance{Weapon::Type::Melee, "Tomb Lance", 2, 3, 3, 3, -1, RAND_D3},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathly Charge                   Yes
// Lord of Bones                    Yes
//

} // namespace Soulblight
