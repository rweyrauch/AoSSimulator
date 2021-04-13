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

        static int ComputePoints(int numModels);

        static void Init();

        EndrinmasterWithEndrinharness();

        ~EndrinmasterWithEndrinharness() override = default;

    protected:

        void configure();

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

