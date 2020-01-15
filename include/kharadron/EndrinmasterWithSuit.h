/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ENDRINMASTERSUIT_H
#define ENDRINMASTERSUIT_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class EndrinmasterWithDirigibleSuit : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    EndrinmasterWithDirigibleSuit();

    ~EndrinmasterWithDirigibleSuit() override = default;

    bool configure();

protected:

private:

    Weapon m_aethercannon,
        m_weaponBattery,
        m_gaze,
        m_saw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Endrinmaster                     No
// Hitcher                          No
// By Grungni, I Have My Eye On You!    No
//

} // namespace KharadronOverlords

#endif //ENDRINMASTERSUIT_H
