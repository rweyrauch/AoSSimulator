/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMWHEEL_H
#define DOOMWHEEL_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Doomwheel : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Doomwheel();

        ~Doomwheel() override = default;

        bool configure();

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onRestore() override;

        void onEndShooting(PlayerId player) override;

    private:

        Weapon m_warpBolts,
                m_grindingWheel,
                m_teethAndKnives;

        mutable bool m_moreMoreFailed = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rolling Doom                     TODO
// More-more Speed!                 TODO
// More-more Warp Bolts!            Yes
//

} // namespace Skaven

#endif //DOOMWHEEL_H