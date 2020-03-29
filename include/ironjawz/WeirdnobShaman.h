/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WEIRDNOBSHAMAN_H
#define WEIRDNOBSHAMAN_H

#include "Ironjawz.h"
#include <Weapon.h>

namespace Ironjawz {

    class OrrukWeirdnobShaman : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukWeirdnobShaman();

        ~OrrukWeirdnobShaman() override = default;

        bool configure();

    protected:

    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brutal Power                     TODO
// Green Puke                       TODO

} // namespace Ironjawz

#endif //WEIRDNOBSHAMAN_H
