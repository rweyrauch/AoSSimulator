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

    class WightKingWithBalefulTombBlade : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WightKingWithBalefulTombBlade(CursedBloodline bloodline, bool hasSteed, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WightKingWithBalefulTombBlade() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

    private:

        bool m_hasSteed = false;

        Weapon m_balefulTombBlade,
                m_steedsHoovesAndTeeth;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beheading Strike                 Yes
// Deathly Invocation               Yes
// Lord of Bones                    TODO
//

} // namespace Soulblight
