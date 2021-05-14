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

    class BloodseekerPalanquin : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodseekerPalanquin(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~BloodseekerPalanquin() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_wail,
                m_blade,
                m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// A Fine Vintage                   TODO
// Wail of the Damned               Yes
// Deathly Invocation               Yes
// Blood Siphon                     TODO
//

} // namespace Soulblight
