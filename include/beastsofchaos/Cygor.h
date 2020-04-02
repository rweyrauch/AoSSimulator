/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CYGOR_H
#define CYGOR_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Cygor : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Cygor();

        ~Cygor() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

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

#endif //CYGOR_H