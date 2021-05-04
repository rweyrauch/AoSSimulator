/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class DreadbladeHarrow : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DreadbladeHarrow(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~DreadbladeHarrow() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_dreadblade,
                m_hoovesAndTeeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Phantasmal Discorporation        TODO
// Dreadblade                       Yes
// Curse of Loyalty                 Yes
//


} // namespace Nighthaunt

