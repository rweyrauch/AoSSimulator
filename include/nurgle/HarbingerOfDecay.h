/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HARBINGEROFDECAY_H
#define HARBINGEROFDECAY_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class HarbingerOfDecay : public NurgleBase
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HarbingerOfDecay();
    ~HarbingerOfDecay() override = default;

    bool configure();

protected:


private:

    Weapon m_scythe,
        m_bite;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Soulbound Shield                 TODO
// Rotsword                         TODO
// Morbid Vigour                    TODO
//

} // Nurgle

#endif //HARBINGEROFDECAY_H
