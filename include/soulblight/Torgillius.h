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

    class TorgilliusTheChamberlain : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TorgilliusTheChamberlain(Lore lore, bool isGeneral);

        ~TorgilliusTheChamberlain() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_clawsAndFangs{Weapon::Type::Melee, "Claws and Fangs", 1, 4, 5, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mastery of Grave-sand            Yes
// Trusted Lieutenant               Yes
// Necrotising Bolt                 Yes
//

} // namespace Soulblight
