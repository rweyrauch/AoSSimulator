/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPPERDACTYLRIDERS_H
#define RIPPERDACTYLRIDERS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class RipperdactylRiders : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        RipperdactylRiders();

        ~RipperdactylRiders() override = default;

        bool configure(int numModels);

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRestore() override { m_usedToad = false; }

    private:

        mutable bool m_usedToad = false;

        Weapon m_spear,
                m_spearAlpha,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               Yes
// Toad Rage                        Yes
//

} // namespace Seraphon

#endif //RIPPERDACTYLRIDERS_H