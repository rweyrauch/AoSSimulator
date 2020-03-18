/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOABROTSPAWNED_H
#define BLOABROTSPAWNED_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class BloabRotspawned : public NurgleBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BloabRotspawned();
    ~BloabRotspawned() override = default;

    bool configure();

protected:


private:

    Weapon m_bile,
        m_scythe,
        m_claws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

} // Nurgle

#endif //BLOABROTSPAWNED_H
