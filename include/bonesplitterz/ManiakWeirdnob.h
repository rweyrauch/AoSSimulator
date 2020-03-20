/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef MANIAKWEIRDNOB_H
#define MANIAKWEIRDNOB_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {

    class ManiakWeirdnob : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ManiakWeirdnob();

        ~ManiakWeirdnob() override = default;

        bool configure();

    protected:


    private:

        Weapon m_bonebeastStaff,
            m_tusksAndHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tusker Charge                    TODO
// Weird Squig                      TODO
// Bone Spirit                      TODO
//

} // namespace Bonesplitterz

#endif //MANIAKWEIRDNOB_H