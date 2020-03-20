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

namespace Nurgle {

    class EpidemiusTallymanOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        EpidemiusTallymanOfNurgle();

        ~EpidemiusTallymanOfNurgle() override = default;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds) override;

    private:

        Weapon m_balesword,
                m_teeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Nurgle's Tallyman                TODO
//

} // Nurgle

#endif //EPIDEMIUS_H
