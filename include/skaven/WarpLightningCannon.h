/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARPLIGHTNINGCANNON_H
#define WARPLIGHTNINGCANNON_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class WarpLightningCannon : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WarpLightningCannon();

        ~WarpLightningCannon() override = default;

        bool configure();

    protected:

        int generateMortalWounds(const Unit *unit) override;

    private:

        Weapon m_warpLightningBlast,
                m_teethAndKnives;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Warp Lightning Blast             Yes
// More-more Warp Lightning         Yes
//

} // namespace Skaven

#endif //WARPLIGHTNINGCANNON_H