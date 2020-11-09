/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ISHARANNTIDECASTER_H
#define ISHARANNTIDECASTER_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class IsharannTidecaster : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        IsharannTidecaster();

        ~IsharannTidecaster() override = default;

        bool configure(Lore lore);

    protected:


    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Spirit Guardians                 TODO
// The Wide Ethersea                TODO
// Riptide                          TODO
//

} // namespace IdonethDeepkin

#endif // ISHARANNTIDECASTER_H
