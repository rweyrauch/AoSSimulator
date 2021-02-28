/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class OgroidThaumaturge : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OgroidThaumaturge();

        ~OgroidThaumaturge() override = default;

        bool configure(Lore lore);

    protected:

        void onCharged() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_staff,
                m_horns,
                m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brutal Rage                      Yes
// Mighty Rampage                   Yes
// Choking Tendrils                 Yes
//

} // namespace Tzeentch

