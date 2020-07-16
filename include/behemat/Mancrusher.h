/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef MANCRUSHER_H
#define MANCRUSHER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Mancrusher : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Mancrusher();

        ~Mancrusher() override = default;

        bool configure(int numModels);

    private:

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace SonsOfBehemat

#endif //MANCRUSHER_H