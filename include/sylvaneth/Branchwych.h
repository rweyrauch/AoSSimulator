/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BRANCHWYCH_H
#define BRANCHWYCH_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Branchwych : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Branchwych();

        ~Branchwych() override = default;

        bool configure();

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_greenwoodScythe,
                m_bittergrubsMandibles;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fury of the Forest               Yes
// Quick Tempered                   Yes
// Unleash Spites                   TODO
//

} // namespace Sylvaneth

#endif //BRANCHWYCH_H