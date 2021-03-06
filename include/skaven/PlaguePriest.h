/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class PlaguePriest : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PlaguePriest(CommandTrait trait, Artefact artefact, bool isGeneral);

        ~PlaguePriest() override = default;

    protected:

        void onEndCombat(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_staff,
                m_censer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Plaque Prayers
//   Disease-disease!               Yes
//   Pestilence-pestilence!         TODO
// Frenzied Assault                 Yes
// Poisonous Fumes                  Yes
//

} // namespace Skaven
