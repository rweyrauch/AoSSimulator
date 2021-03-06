/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightQuestor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        KnightQuestor(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KnightQuestor() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_warblade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thundercharged Strike            Yes
// Heroic Challenge                 Yes
// Sigmarite Shield                 Yes
//

} // namespace StormcastEternals
