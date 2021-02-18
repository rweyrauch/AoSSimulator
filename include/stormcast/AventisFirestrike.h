/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class AventisFirestrike : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        AventisFirestrike();

        ~AventisFirestrike() override = default;

        bool configure(Lore lore);

    protected:

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

        void onCharged() override;

        void onStartHero(PlayerId player) override;

    private:

        bool m_shatteredFlasks = false;

        Weapon m_staffOfHammerhal,
                m_hornsAndHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Comet Trail                      TODO
// Cycle of the Storm               TODO
// Meteoric Strike                  Yes
// Spirit Flask                     Yes
// Thunderhead Crown                Yes
// Righteous Indignation            TODO
// Pyroelectric Blast               Yes
// Prime Electrids                  Yes
// Fiery Orator                     Yes

} // namespace StormcastEternals
