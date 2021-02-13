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

    class GlutosOrscollion : public SlaaneshBase {
    public:

        enum WeaponOption {
            Living_Whip,
            Shining_Aegis
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        GlutosOrscollion();

        ~GlutosOrscollion() override = default;

        bool configure(Lore lore);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

    private:

        int woundsTaken() const {
            return wounds() - remainingWounds();
        }

        Weapon m_greatblade,
                m_scourge,
                m_dagger,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Grand Gourmand               TODO
// Fog of Temptation                TODO
// The Leerstave of Loth'shar       TODO
// Gorge on Excess                  TODO
// Crippling Famishment             TODO
// Companions
//  Painbringer Kyazu               TODO
//  Lashmaster Vhyssk               TODO
//  Priestess Dolece                TODO
//

} // Slannesh
