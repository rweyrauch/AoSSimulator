/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPITECLAWSSWARM_H
#define SPITECLAWSSWARM_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class SpiteclawsSwarm : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpiteclawsSwarm();

        ~SpiteclawsSwarm() override = default;

        bool configure();

    protected:

    private:

        Weapon m_stabbingBlade,
                m_rustyFlail,
                m_rustySpear,
                m_punchDaggers;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aversion to Death                TODO
//

} // namespace Skaven

#endif //SPITECLAWSSWARM_H
