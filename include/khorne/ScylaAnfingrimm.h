/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class ScylaAnfingrimm : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ScylaAnfingrimm() = delete;

        ~ScylaAnfingrimm() override = default;

    protected:

        ScylaAnfingrimm(SlaughterHost host);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_brutalFists,
                m_serpentineTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brass Collar of Khorne           Yes
// Raging Fury                      Yes
// Bestial Leap                     Yes
//

} // namespace Khorne
