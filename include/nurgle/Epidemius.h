/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EPIDEMIUS_H
#define EPIDEMIUS_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class EpidemiusTallymanOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    EpidemiusTallymanOfNurgle();
    ~EpidemiusTallymanOfNurgle() override = default;

    bool configure();

protected:


private:

    Weapon m_balesword,
        m_teeth;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           TODO
// Nurgle's Tallyman                TODO
//

} // Nurgle

#endif //EPIDEMIUS_H
