/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class TheMasque : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheMasque();

        ~TheMasque() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_ravagingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Staff of Masks                   Yes
// The Endless Dance                Yes
// Lithe and Swift                  Yes
// Inhuman Reflexes                 Yes
//

} // Slannesh
