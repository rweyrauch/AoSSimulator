/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Razordons : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Razordons();

        ~Razordons() override = default;

        bool configure(int numModels);

    protected:

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_spikes,
                m_tail,
                m_goad;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Piercing Barbs                   Yes
// Instinctive Defense              TODO
//

} // namespace Seraphon
