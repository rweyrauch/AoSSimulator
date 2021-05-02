/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz {

    class MorgoksKrushas : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MorgoksKrushas();

        ~MorgoksKrushas() override = default;

    protected:

        void configure();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    protected:

        Weapon m_krushaWeaponsMorgok,
                m_krushaWeapons,
                m_goreBasha;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Duff Up da Big Thing             Yes
//

} // namespace Ironjawz
