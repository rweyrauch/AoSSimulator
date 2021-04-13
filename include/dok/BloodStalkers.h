/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class BloodStalkers : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodStalkers();

        ~BloodStalkers() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_heartseekerBow,
                m_scianlar,
                m_bloodWyrm;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Heartseekers                     Yes
//

} // namespace DaughtersOfKhaine
