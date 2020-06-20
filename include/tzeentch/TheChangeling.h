/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THECHANGELING_H
#define THECHANGELING_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class TheChangeling : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheChangeling();

        ~TheChangeling() override = default;

        bool configure(Lore lore);

    protected:

    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Puckish Misdirection             TODO
// Arch-deceiver                    TODO
//

} // namespace Tzeentch

#endif //THECHANGELING_H
