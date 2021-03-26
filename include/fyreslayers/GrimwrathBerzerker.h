/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class GrimwrathBerzerker : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GrimwrathBerzerker();

        ~GrimwrathBerzerker() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_throwingAxe,
                m_greatAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Berzerker            Yes
// Battle-fury                      Yes
// Dead, But Not Defeated           Yes
//

} // namespace Fyreslayers
