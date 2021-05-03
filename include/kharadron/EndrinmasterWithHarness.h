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

    class EndrinmasterWithEndrinharness : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        EndrinmasterWithEndrinharness(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~EndrinmasterWithEndrinharness() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_gaze,
                m_hammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Endrinmaster                     Yes
// Endrinharness                    Yes
// By Grungni, I Have My Eye On You!    TODO
//

} // namespace KharadronOverlords

