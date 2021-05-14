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

    class VampireLordOnZombieDragon : public SoulblightBase {
    public:

        enum WeaponOption {
            Deathlance,
            Vampiric_Sword
        };

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        VampireLordOnZombieDragon(Legion legion, WeaponOption option, bool shield, bool chalice, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~VampireLordOnZombieDragon() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_breath,
                m_deathlance,
                m_sword,
                m_maw,
                m_claws;

        bool m_haveChaliceOfBlood = false;
        bool m_usedChaliceOfBlood = false;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              Yes
// The Hunger                       Yes
// Deathlance Charge                Yes
// Anciet Shield                    Yes
// Chalice of Blood                 Yes
// Deathly Invocation               Yes
// Blood Boil                       TODO
// Dread Knights                    TODO
//

} // namespace Soulblight
