/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class AbhorrantGhoulKingOnTerrorgheist : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AbhorrantGhoulKingOnTerrorgheist(GrandCourt court, Delusion delusion, Lore lore, MountTrait mountTrait, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~AbhorrantGhoulKingOnTerrorgheist() override = default;

        void onFriendlyUnitSlain(const Unit *attacker) override;

    protected:

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onWounded() override;

        void onRestore() override;

        void onStartShooting(PlayerId player) override;

        size_t getDamageTableIndex() const;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        MountTrait m_mountTrait = MountTrait::None;

        Weapon m_deathShriek,
                m_goryTalonsAndFangs,
                m_skeletalClaws,
                m_fangedMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Death Shriek                     Yes
// Gaping Maw                       Yes
// Infested                         Yes
// Royal Blood                      Yes
// Unholy Vitality                  Yes
// Summon Royal Guard               Yes
//

} // namespace FleshEaterCourt
