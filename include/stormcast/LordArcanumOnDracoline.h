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

    class LordArcanumOnDracoline : public MountedStormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        LordArcanumOnDracoline(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral);

        ~LordArcanumOnDracoline() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls chargeRerolls() const override;

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

        int supernaturalRoar(const Unit *target);

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave,
                m_monstrousClaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cycle of the Storm               TODO
// Supernatural Roar                Yes
// Thunderous Pounce                Yes
// Storm Lance                      Yes
// Prime Electrids                  Yes
// Pack Alpha                       TODO
// Spirit Flask                     Yes

} // namespace StormcastEternals
