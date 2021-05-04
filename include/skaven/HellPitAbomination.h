/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class HellPitAbomination : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        HellPitAbomination();

        ~HellPitAbomination() override;

    protected:

        void onStartHero(PlayerId player) override;

        int generateMortalWounds(const Unit *unit) override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        int terrifying(const Unit *unit);

        size_t getDamageTableIndex() const;

    protected:

        bool m_beenSlain = false;

    private:

        Weapon m_gnashingTeath,
                m_flailingFists,
                m_avalancheOfFlesh;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Avalanche of Flesh               Yes
// Regenerating Monstrosity         Yes
// Terrifying                       Yes
// Warpstone Spikes                 TODO
// Too Horrible to Die              Yes
//

} // namespace Skaven
