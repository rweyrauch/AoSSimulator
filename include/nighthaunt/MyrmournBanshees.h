/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MYRMOURNBANSHEES_H
#define MYRMOURNBANSHEES_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class MyrmournBanshees : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MyrmournBanshees();

        ~MyrmournBanshees() override = default;

        bool configure(int numModels);


    private:

        Weapon m_dagger;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         TODO
// Spell Eater                      TODO
//

} // namespace Nighthaunt

#endif //MYRMOURNBANSHEES_H