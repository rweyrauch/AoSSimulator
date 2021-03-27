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

    class OrrukWarbossOnWyvern : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        OrrukWarbossOnWyvern();

        ~OrrukWarbossOnWyvern() override = default;

    protected:

        void configure(bool pairedChoppas);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        void onRestore() override;

    private:

        bool m_pairedChoppas = false;

        Weapon m_bossChoppa,
                m_hornsClawsAndTeeth,
                m_barbedTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Agonising Venom                  TODO
// Boss Shield                      Yes
// Choppa Boss                      Yes
// Waaagh!                          TODO
//

} // namespace Greenskinz
