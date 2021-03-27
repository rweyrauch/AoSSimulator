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

    class LordArcanum : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        LordArcanum();

        ~LordArcanum() override = default;

    protected:

        void configure(Lore lore);

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cycle of the Storm               TODO
// Spirit Flask                     Yes
// Thunderclap                      Yes
// Aetheric Manipulation            TODO
// Prime Electrids                  Yes

} // namespace StormcastEternals
