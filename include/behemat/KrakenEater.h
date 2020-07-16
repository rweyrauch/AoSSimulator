/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef KRAKENEATER_H
#define KRAKENEATER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class KrakenEater : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KrakenEater();

        ~KrakenEater() override = default;

        bool configure();

    private:

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace SonsOfBehemat

#endif //KRAKENEATER_H