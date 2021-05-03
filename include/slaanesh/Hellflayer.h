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

    class Hellflayer : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Hellflayer(Host host);

        ~Hellflayer() override = default;

    protected:

        void onStartCombat(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        int m_extraMeleeAttacks = 0;

        Weapon m_flensingWhips,
                m_piercingClaws,
                m_poisonedTongues;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Soulscent                        Yes
//

} // Slannesh
