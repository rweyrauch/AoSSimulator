/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MorathiTheShadowQueen : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MorathiTheShadowQueen(Temple temple, bool isGeneral);

        ~MorathiTheShadowQueen() override;

    protected:

        void onWounded() override;

        void onRestore() override;

    private:

        size_t getDamageTableIndex() const;

        int furyOfTheShadowQueen(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                 const Unit *target);

    private:

        lsignal::slot m_furySlot;

        Weapon m_gaze,
                m_heartrender,
                m_crown,
                m_tail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fury of the Shadow Queen         Yes
// Two Bodies, One Soul             TODO
// The Iron Heart of Khaine         TODO
//

} // namespace DaughtersOfKhaine
