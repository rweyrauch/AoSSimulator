/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class TheChosenAxes : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        TheChosenAxes();

        ~TheChosenAxes() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_usedBerserkFury = false;
        bool m_berserkFuryActive = false;

        Weapon m_handaxes,
                m_handaxesTefk,
                m_greatAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chosen Kin                       Yes
// Berserk Fury                     Yes
// Fyresteel Handaxes               Yes
//

} // namespace Fyreslayers
