/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class SlaangorFiendbloods : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SlaangorFiendbloods(Host host, int numModels, int points);

        ~SlaangorFiendbloods() override = default;

        bool configure(int numModels);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_claws,
                m_gildedWeapon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Slaughter At Any Cost            Yes
// Obsessive Violence               Yes
//

} // Slannesh
