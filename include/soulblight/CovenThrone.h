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

    class CovenThrone : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CovenThrone(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~CovenThrone() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_bite,
                m_stiletto,
                m_poniards,
                m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// Scrying Pool                     TODO
// The Hunger                       Yes
// Shudder                          TODO
// Tactful Insight                  TODO
//

} // namespace Soulblight
