/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class EndrinmasterWithDirigibleSuit : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        EndrinmasterWithDirigibleSuit(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~EndrinmasterWithDirigibleSuit() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_aethercannon,
                m_weaponBattery,
                m_gaze,
                m_saw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Endrinmaster                     Yes
// Hitcher                          TODO
// By Grungni, I Have My Eye On You!    TODO
//

} // namespace KharadronOverlords

