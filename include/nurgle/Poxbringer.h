/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef POXBRINGER_H
#define POXBRINGER_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class PoxbringerHeraldOfNurgle : public NurgleBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    PoxbringerHeraldOfNurgle();
    ~PoxbringerHeraldOfNurgle() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_balesword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disqustingly Resilient           Yes
// In Death There is Life           TODO
// Eruptive Infestation             TODO
//

} // Nurgle

#endif //POXBRINGER_H