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

    class AveronStormsire : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        AveronStormsire(Lore lore, bool isGeneral);

        ~AveronStormsire() override = default;

    protected:

        void onStartCombat(PlayerId player) override;

        void onRestore() override {
            m_shatteredFlasks = false;
            m_usedVoidstormScroll = false;
        }

        int unbindingModifier() const override;

    private:

        bool m_shatteredFlasks = false;
        mutable bool m_usedVoidstormScroll = false;

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voidstorm Scroll                 Yes
// Spirit Flask                     Yes
// Stormsire                        Yes
//

} // namespace StormcastEternals
