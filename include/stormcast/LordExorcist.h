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

    class LordExorcist : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        LordExorcist(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordExorcist() override = default;

    protected:

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_stave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Redemptor Casket                 Yes
// Purifying Blast                  Yes
//

} // namespace StormcastEternals
