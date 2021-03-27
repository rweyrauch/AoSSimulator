/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class RipperdactylChief : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        RipperdactylChief();

        ~RipperdactylChief() override = default;

    protected:

        void configure();

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_skyblade,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               Yes
// Ripperdactyl Assault             TODO
//

} // namespace Seraphon
