/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Branchwych : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Branchwych(Glade glade, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        Branchwych() = delete;

        ~Branchwych() override = default;

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
// Unleash Spites                   Yes
//

} // namespace Sylvaneth
