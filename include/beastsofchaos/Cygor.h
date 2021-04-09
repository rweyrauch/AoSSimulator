/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Cygor : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Cygor() = delete;

        ~Cygor() override = default;

        explicit Cygor(Greatfray fray);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

    private:

        Weapon m_desecratedBoulder,
                m_massiveHorns;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Soul-eater                       Yes
// Ghostsight                       Yes
//

} // namespace BeastsOfChaos
