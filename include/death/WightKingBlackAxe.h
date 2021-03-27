/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class WightKingWithBlackAxe : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WightKingWithBlackAxe();

        ~WightKingWithBlackAxe() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnitk) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_blackAxe;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Black Axe                        TODO
// Barrow Armour                    Yes
// Deathly Invocation               Yes
// Lord of Bones                    TODO
//

} // namespace Death
