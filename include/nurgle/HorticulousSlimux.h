/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HORTICULOUSSLIMUX_H
#define HORTICULOUSSLIMUX_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class HorticulousSlimux : public NurgleBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HorticulousSlimux();
    ~HorticulousSlimux() override = default;

    bool configure();

protected:


private:

    Weapon m_shears,
        m_jaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           TODO
// Acidic Slime Trail               TODO
// Beast Handler                    TODO
// In Death There is Life           TODO
// Cultivating the Garden of Nurgle TODO
//

} // Nurgle

#endif //HORTICULOUSSLIMUX_H
