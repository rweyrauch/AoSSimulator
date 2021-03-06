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

    class BladebringerOnSeekerChariot : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BladebringerOnSeekerChariot(Host host, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~BladebringerOnSeekerChariot() override = default;

    protected:

        void onCharged() override;

    private:

        Weapon m_flensingWhips,
                m_piercingClaws,
                m_poisonedTongues;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Impossibly Swift                 Yes
// Mutilating Blades                Yes
// Acquiescence                     Yes
//

} // Slannesh
