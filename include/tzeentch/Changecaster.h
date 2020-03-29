/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHANGECASTER_H
#define CHANGECASTER_H


#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class ChangecasterHeraldOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChangecasterHeraldOfTzeentch();

        ~ChangecasterHeraldOfTzeentch() override = default;

        bool configure();

    protected:

    private:

        Weapon m_flames,
                m_staff,
                m_dagger;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fortune and Fate                 TODO
// Arcane Tome                      TODO
// Pink Fire of Tzeentch            TODO
//

} // namespace Tzeentch

#endif //CHANGECASTER_H
