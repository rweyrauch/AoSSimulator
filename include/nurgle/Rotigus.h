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

    class Rotigus : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Rotigus(PlagueLegion legion, Lore lore, bool isGeneral);

        ~Rotigus() override = default;

    protected:

        void onWounded() override;

        void onCharged() override;

        size_t getDamageTableIndex() const;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_gnarlrod,
                m_fangedMaw,
                m_nurglings;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blubber and Bile                 Yes
// Corpulent Mass                   Yes
// Mountain of Loathsome Flesh      Yes
// Streams of Brackish Filth        Yes
// Deluge of Nurgle                 TODO
//

} // Nurgle
