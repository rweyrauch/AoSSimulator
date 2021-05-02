/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class PlagueCenserBearers : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PlagueCenserBearers(int points);

        ~PlagueCenserBearers() override = default;

        bool configure(int numModels);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls battleshockRerolls() const override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_censer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frenzied Assault                 Yes
// Plague Disciples                 Yes
// Poisonous Fumes                  Yes
//

} // namespace Skaven
