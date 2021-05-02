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

    class KnightIncantor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        KnightIncantor(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KnightIncantor() override = default;

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
// Spirit Storm                     Yes
//

} // namespace StormcastEternals
