/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MYARILGHTCALLER_H
#define MYARILGHTCALLER_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class MyariLigthcaller : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MyariLigthcaller();

        ~MyariLigthcaller() override = default;

        bool configure();

    protected:

    private:

        Weapon m_beams,
            m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scryowl Familar                  TODO
// Dazzling Light                   TODO
//

} // namespace LuminethRealmLords

#endif // MYARILGHTCALLER_H