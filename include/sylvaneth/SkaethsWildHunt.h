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

    class SkaethsWildHunt : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkaethsWildHunt();

        ~SkaethsWildHunt() override = default;

    protected:

        void configure();

    private:

        Weapon m_seekerBow,
                m_javalin,
                m_javalinMelee,
                m_huntingWeapon,
                m_teethAndClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fleet of Foot                    Yes
// Might of Kurnoth                 TODO
//

} // namespace Sylvaneth
