/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/MountedStormcast.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordArcanumOnGryphcharger : public MountedStormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        LordArcanumOnGryphcharger(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral);

        ~LordArcanumOnGryphcharger() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave,
                m_beakAndClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethereal Strike                 Yes
// Ride the Winds Aetheric          TODO
// Spirit Flask                     Yes
// Healing Light                    Yes
// Prime Electrids                  Yes
// Soul Energy                      TODO

} // namespace StormcastEternals
