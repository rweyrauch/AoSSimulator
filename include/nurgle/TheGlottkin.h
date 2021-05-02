/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class TheGlottkin : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheGlottkin();

        ~TheGlottkin() override = default;

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_pestilentTorrent,
                m_flailingTentacle,
                m_lampreyMaw,
                m_poisonScythe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mountain of Loathsome Flesh      Yes
// Blessing of Nurgle               Yes
// Horrific Opponent                TODO
// Fleshy Abundance                 TODO
// Lords of Nurgle                  TODO
//

} // Nurgle
