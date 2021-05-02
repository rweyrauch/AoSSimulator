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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ScylaAnfingrimm() = delete;

        ~ScylaAnfingrimm() override = default;

    protected:

        ScylaAnfingrimm(SlaughterHost host);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_brutalFists{Weapon::Type::Melee, "Brutal Fists", 2, RAND_2D6, 4, 3, -1, 1},
                m_serpentineTail{Weapon::Type::Melee, "Serpentine Tail", 3, 2, 3, 3, -1, RAND_D3};

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
