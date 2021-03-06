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

    class LordOfAfflictions : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordOfAfflictions(PlagueLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordOfAfflictions() override;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartHero(PlayerId player) override;

        Rerolls plagueVectorToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_festerspike,
                m_mouthparts,
                m_sting,
                m_tocsin;

        lsignal::slot m_plagueVectorSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Rotten Regeneration              Yes
// Plague Vector                    Yes
// Incubatch                        Yes
// Virulent Discharge               Yes
// Spearhead of Contagion           TODO
//

} // Nurgle
