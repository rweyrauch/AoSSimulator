/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class SigvaldPrinceOfSlaanesh : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SigvaldPrinceOfSlaanesh();

        ~SigvaldPrinceOfSlaanesh() override = default;

    protected:

        void configure();

        // Powered by Vainglory
        int chargeModifier() const override { return 3; }

        void onCharged() override;

        void onBeginTurn(int battleRound, PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_shardslash;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Glorious Reborn                  TODO
// Powered by Vainglory             Yes
// Shardslash                       TODO
// The Mirror Shield                Yes
//

} // Slannesh
