/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFAFFLICTIONS_H
#define LORDOFAFFLICTIONS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class LordOfAfflictions : public NurgleBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    LordOfAfflictions();
    ~LordOfAfflictions() override = default;

    bool configure();

protected:


private:

    Weapon m_festerspike,
        m_mouthparts,
        m_sting,
        m_tocsin;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           TODO
// Rotten Regeneration              TODO
// Plague Vector                    TODO
// Incubatch                        TODO
// Virulent Discharge               TODO
// Spearhead of Contagion           TODO
//

} // Nurgle

#endif //LORDOFAFFLICTIONS_H