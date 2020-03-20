/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VARGHEISTS_H
#define VARGHEISTS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {
    class Vargheists : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Vargheists();

        ~Vargheists() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_fangsAndTalons,
            m_fangsAndTalonsVargoyle;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood-maddened Frenzy            TODO
//

} //namespace Death

#endif //VARGHEISTS_H