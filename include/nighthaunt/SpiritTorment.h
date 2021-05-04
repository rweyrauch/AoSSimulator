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

    class SpiritTorment : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SpiritTorment(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral);

        SpiritTorment() = delete;

        ~SpiritTorment() override;

    protected:

        Rerolls nagashsBidding(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_chains{Weapon::Type::Melee, "Shacklegheist Chains", 2, 3, 4, 3, -2, RAND_D3};

        lsignal::slot m_nagashsBiddingSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Nagash's Bidding                 Yes
// Captured Soul Energy             TODO
//

} // namespace Nighthaunt

