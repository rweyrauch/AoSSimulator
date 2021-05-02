/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class GauntSummonerOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GauntSummonerOfTzeentch(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~GauntSummonerOfTzeentch() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onRestore() override;

    private:

        Weapon m_staff,
                m_blade;

        bool m_usedBookOfSecrets = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Warptongue Blade                 Yes
// Book of Profane Secrets          Yes
// Infernal Flames                  Yes
//

} // namespace Tzeentch

