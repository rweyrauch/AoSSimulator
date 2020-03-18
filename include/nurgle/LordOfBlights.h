/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFBLIGHTS_H
#define LORDOFBLIGHTS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class LordOfBlights : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    LordOfBlights();
    ~LordOfBlights() override = default;

    bool configure();

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    Weapon m_ripenedDeathsHead,
        m_bountyDeathsHead,
        m_hammer;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Munificent Bounty                TODO
// Vermid Shield                    Yes
// Plague of Flies                  TODO
//

} // Nurgle

#endif //LORDOFBLIGHTS_H