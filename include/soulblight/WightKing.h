/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class WightKing : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WightKing(CursedBloodline bloodline, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WightKing() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_balefulTombBlade;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beheading Strike                 Yes
// Lord of Bones                    Yes
//

} // namespace Soulblight
