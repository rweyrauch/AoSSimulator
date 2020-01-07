/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ENDRINMASTERHARNESS_H
#define ENDRINMASTERHARNESS_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class EndrinmasterWithEndrinharness : public KharadronBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    EndrinmasterWithEndrinharness();

    ~EndrinmasterWithEndrinharness() override = default;

    bool configure();

protected:

private:

    Weapon m_gaze,
        m_hammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Endrinmaster                     No
// Endrinharness                    No
// By Grungni, I Have My Eye On You!    No
//

} // namespace KharadronOverlords

#endif //ENDRINMASTERHARNESS_H
