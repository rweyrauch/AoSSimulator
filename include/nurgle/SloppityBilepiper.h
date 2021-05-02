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

    class SloppityBilepiperHeraldOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SloppityBilepiperHeraldOfNurgle();

        ~SloppityBilepiperHeraldOfNurgle() override;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int diseaseOfMirthBraveryMod(const Unit *unit);

    private:

        Weapon m_marotter;

        lsignal::slot m_diseaseOfMirthSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Disease of Mirth                 Yes
// Jolly Gutpipes
//   A Stabbing We Will Go!                     TODO
//   Early One Evening My Pustule Was Seeping   TODO
//   My Love Is Like a Ripe, Ripe Fart          TODO
//

} // Nurgle
