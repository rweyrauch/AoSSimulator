/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef WARDOKK_H
#define WARDOKK_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {

    class Wardokk : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Wardokk();

        ~Wardokk() override = default;

        bool configure();

    protected:


    private:

        Weapon m_bonebeastStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ritual Dance                     TODO
//     Grimdokk
//     Glyphdokk
//     Weirddokk
//

} // namespace Bonesplitterz

#endif //WARDOKK_H