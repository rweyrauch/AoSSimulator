/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class GuardianOfSouls : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GuardianOfSouls(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        GuardianOfSouls() = delete;

        ~GuardianOfSouls() override;

    protected:

        int nightmareLantern(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Chill Blade", 1, 3, 3, 3, -1, 1},
                m_maul{Weapon::Type::Melee, "Maul of Judgement", 1, 2, 3, 3, 0, 2};

        lsignal::slot m_nightmareLanternSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Nightmare Lantern                Yes
// Spectral Lure                    Yes
//


} // namespace Nighthaunt
