/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELLPITABOMINATION_H
#define HELLPITABOMINATION_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class HellPitAbomination : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HellPitAbomination();

        ~HellPitAbomination() override;

        bool configure();

    protected:

        void onStartHero(PlayerId player) override;

        int generateMortalWounds(const Unit *unit) override;

        void onSlain() override;

        int terrifying(const Unit *unit);

        int getDamageTableIndex() const;

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

#endif //HELLPITABOMINATION_H