/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORBIDEXTWICEBORN_H
#define MORBIDEXTWICEBORN_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class MorbidexTwiceborn : public NurgleBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    MorbidexTwiceborn();
    ~MorbidexTwiceborn() override = default;

    bool configure();

protected:


private:

    Weapon m_tongues,
        m_scythe,
        m_claws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Lord of Nurglings                TODO
// Malicious Mites                  TODO
// Nurgle's Rot                     TODO
// Repugnant Regrowth               TODO
//

} // Nurgle

#endif //MORBIDEXTWICEBORN_H
