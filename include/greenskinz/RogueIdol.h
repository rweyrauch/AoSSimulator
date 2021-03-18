/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Greenskinz {

    class RogueIdol : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        RogueIdol();

        ~RogueIdol() override = default;

        bool configure();

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onEndCombat(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_boulderFists,
                m_stompinFeet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Avalanche!                       Yes
// Da Big' Un                       Yes
// Livin' Idol                      TODO
// Rubble and Ruin                  Yes
// Spirit of the Waaagh!            Yes
//

} // namespace Greenskinz
