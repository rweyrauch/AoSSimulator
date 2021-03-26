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

    class AbhorrantGhoulKingOnZombieDragon : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AbhorrantGhoulKingOnZombieDragon();

        ~AbhorrantGhoulKingOnZombieDragon() override = default;

        bool configure(Lore lore, MountTrait trait);

    protected:

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onWounded() override;

        void onRestore() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        size_t getDamageTableIndex() const;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        MountTrait m_mountTrait = MountTrait::None;

        Weapon m_pestilentialBreath,
                m_goryTalonsAndFangs,
                m_snappingMaw,
                m_swordlikeClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              Yes
// Royal Blood                      Yes
// Malefic Hunger                   Yes
// Summon Courtier                  Yes
//

} // namespace FleshEaterCourt
