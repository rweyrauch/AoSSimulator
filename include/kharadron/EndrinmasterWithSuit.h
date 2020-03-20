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

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Endrinmaster                     TODO
// Hitcher                          TODO
// By Grungni, I Have My Eye On You!    TODO
//

} // namespace KharadronOverlords

#endif //ENDRINMASTERSUIT_H
