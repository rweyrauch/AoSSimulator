/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVENTISFIRESTRIKE_H
#define AVENTISFIRESTRIKE_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
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

        bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

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
// Fiery Orator                     TODO

} // namespace StormcastEternals

#endif //AVENTISFIRESTRIKE_H